#include "config.h"
#include "tl_ucp.h"
#include "allreduce.h"
#include "utils/ucc_coll_utils.h"
#include "core/ucc_progress_queue.h"
#include "tl_ucp_sendrecv.h"

ucc_base_coll_alg_info_t
    ucc_tl_ucp_allreduce_algs[UCC_TL_UCP_ALLREDUCE_ALG_LAST + 1] = {
        [UCC_TL_UCP_ALLREDUCE_ALG_KNOMIAL] =
            {.id   = UCC_TL_UCP_ALLREDUCE_ALG_KNOMIAL,
             .name = "knomial",
             .desc =
                 "recursive knomial with arbitrary radix (optimized for latency)"},
        [UCC_TL_UCP_ALLREDUCE_ALG_SRA_KNOMIAL] =
            {.id   = UCC_TL_UCP_ALLREDUCE_ALG_SRA_KNOMIAL,
             .name = "sra_knomial",
             .desc = "recursive knomial scatter-reduce followed by knomial "
                     "allgather (optimized for BW)"},
        [UCC_TL_UCP_ALLREDUCE_ALG_DBT] =
            {.id   = UCC_TL_UCP_ALLREDUCE_ALG_DBT,
             .name = "dbt",
             .desc = "alreduce over double binary tree where a leaf in one tree "
                     "will be intermediate in other (optimized for BW)"},
        [UCC_TL_UCP_ALLREDUCE_ALG_SLIDING_WINDOW] =
            {.id   = UCC_TL_UCP_ALLREDUCE_ALG_SLIDING_WINDOW,
             .name = "sliding_window",
             .desc = "sliding window allreduce (optimized for running on DPU)"},
        [UCC_TL_UCP_ALLREDUCE_ALG_LAST] = {
            .id = 0, .name = NULL, .desc = NULL}};

ucc_status_t ucc_tl_ucp_allreduce_init(ucc_tl_ucp_task_t *task)
{
    ucc_status_t status;
    ALLREDUCE_TASK_CHECK(TASK_ARGS(task), TASK_TEAM(task));
    status = ucc_tl_ucp_allreduce_knomial_init_common(task);
out:
    return status;
}

ucc_status_t ucc_tl_ucp_allreduce_knomial_init(ucc_base_coll_args_t *coll_args,
                                               ucc_base_team_t *     team,
                                               ucc_coll_task_t **    task_h)
{
    ucc_tl_ucp_team_t *tl_team = ucc_derived_of(team, ucc_tl_ucp_team_t);
    ucc_tl_ucp_task_t *task;
    ucc_status_t       status;
    ALLREDUCE_TASK_CHECK(coll_args->args, tl_team);
    task                 = ucc_tl_ucp_init_task(coll_args, team);
    *task_h              = &task->super;
    status = ucc_tl_ucp_allreduce_knomial_init_common(task);
out:
    return status;
}

// เริ่มต้นการทำงานของ All-Reduce แบบ One-Sided
ucc_status_t ucc_tl_ucp_allreduce_onesided_start(ucc_coll_task_t *ctask)
{
    ucc_tl_ucp_task_t *task   = ucc_derived_of(ctask, ucc_tl_ucp_task_t);
    ucc_tl_ucp_team_t *team   = TASK_TEAM(task);
    ptrdiff_t          src    = (ptrdiff_t)TASK_ARGS(task).src.info.buffer;
    ptrdiff_t          dest   = (ptrdiff_t)TASK_ARGS(task).dst.info.buffer;
    size_t             nelems = TASK_ARGS(task).src.info.count;
    ucc_rank_t         grank  = UCC_TL_TEAM_RANK(team);
    ucc_rank_t         gsize  = UCC_TL_TEAM_SIZE(team);
    ucc_rank_t         start  = (grank + 1) % gsize;
    long *             pSync  = TASK_ARGS(task).global_work_buffer;
    ucc_rank_t         peer;

    ucc_tl_ucp_task_reset(task, UCC_INPROGRESS);
    nelems = (nelems / gsize) * ucc_dt_size(TASK_ARGS(task).src.info.datatype);
    dest   = dest + grank * nelems;
    UCPCHECK_GOTO(ucc_tl_ucp_put_nb((void *)(src + start * nelems),
                                    (void *)dest, nelems, start, team, task),
                  task, out);
    UCPCHECK_GOTO(ucc_tl_ucp_atomic_inc(pSync, start, team), task, out);

    for (peer = (start + 1) % gsize; peer != start; peer = (peer + 1) % gsize) {
        UCPCHECK_GOTO(ucc_tl_ucp_put_nb((void *)(src + peer * nelems),
                                        (void *)dest, nelems, peer, team, task),
                      task, out);
        UCPCHECK_GOTO(ucc_tl_ucp_atomic_inc(pSync, peer, team), task, out);
    }

    return ucc_progress_queue_enqueue(UCC_TL_CORE_CTX(team)->pq, &task->super);
out:
    return task->super.status;
}

// ดำเนินการของ All-Reduce แบบ One-Sided
void ucc_tl_ucp_allreduce_onesided_progress(ucc_coll_task_t *ctask)
{
    ucc_tl_ucp_task_t *task  = ucc_derived_of(ctask, ucc_tl_ucp_task_t);
    ucc_tl_ucp_team_t *team  = TASK_TEAM(task);
    ucc_rank_t         gsize = UCC_TL_TEAM_SIZE(team);
    long *             pSync = TASK_ARGS(task).global_work_buffer;

    if (ucc_tl_ucp_test_onesided(task, gsize) == UCC_INPROGRESS) {
        return;
    }

    pSync[0]           = 0;
    task->super.status = UCC_OK;
}