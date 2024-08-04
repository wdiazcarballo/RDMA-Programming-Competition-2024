/**
 * Copyright (c) 2021, NVIDIA CORPORATION & AFFILIATES. All rights reserved.
 *
 * See file LICENSE for terms.
 */
#include "config.h"
#include "tl_ucp.h"
#include "allreduce.h"
#include "utils/ucc_coll_utils.h"

// Define the ucc_tl_ucp_team_t structure
typedef struct ucc_tl_ucp_team {
    ucc_base_team_t super;
    // Add other necessary fields here
} ucc_tl_ucp_team_t;

// Define the ucc_tl_ucp_task_t structure
typedef struct ucc_tl_ucp_task {
    ucc_coll_task_t super;
    // Add other necessary fields here
} ucc_tl_ucp_task_t;

ucc_base_coll_alg_info_t ucc_tl_ucp_allreduce_algs[UCC_TL_UCP_ALLREDUCE_ALG_LAST + 1] = {
    [UCC_TL_UCP_ALLREDUCE_ALG_KNOMIAL] = {
        .id = UCC_TL_UCP_ALLREDUCE_ALG_KNOMIAL,
        .name = "knomial",
        .desc = "recursive knomial with arbitrary radix (optimized for latency)"
    },
    [UCC_TL_UCP_ALLREDUCE_ALG_SRA_KNOMIAL] = {
        .id = UCC_TL_UCP_ALLREDUCE_ALG_SRA_KNOMIAL,
        .name = "sra_knomial",
        .desc = "recursive knomial scatter-reduce followed by knomial allgather (optimized for BW)"
    },
    [UCC_TL_UCP_ALLREDUCE_ALG_DBT] = {
        .id = UCC_TL_UCP_ALLREDUCE_ALG_DBT,
        .name = "dbt",
        .desc = "allreduce over double binary tree where a leaf in one tree will be intermediate in other (optimized for BW)"
    },
    [UCC_TL_UCP_ALLREDUCE_ALG_SLIDING_WINDOW] = {
        .id = UCC_TL_UCP_ALLREDUCE_ALG_SLIDING_WINDOW,
        .name = "sliding_window",
        .desc = "sliding window allreduce (optimized for running on DPU)"
    },
    [UCC_TL_UCP_ALLREDUCE_ALG_LAST] = {
        .id = 0, .name = NULL, .desc = NULL
    }
};

ucc_status_t ucc_tl_ucp_allreduce_init(ucc_tl_ucp_task_t *task)
{
    ucc_status_t status;
    ALLREDUCE_TASK_CHECK(TASK_ARGS(task), TASK_TEAM(task));
    status = ucc_tl_ucp_allreduce_knomial_init_common(task);
out:
    return status;
}

ucc_status_t ucc_tl_ucp_allreduce_knomial_init(ucc_base_coll_args_t *coll_args,
                                               ucc_base_team_t *team,
                                               ucc_coll_task_t **task_h)
{
    ucc_tl_ucp_team_t *tl_team = ucc_derived_of(team, ucc_tl_ucp_team_t);
    ucc_tl_ucp_task_t *task;
    ucc_status_t status;
    ALLREDUCE_TASK_CHECK(coll_args->args, tl_team);
    task = ucc_tl_ucp_init_task(coll_args, team);
    *task_h = &task->super;
    status = ucc_tl_ucp_allreduce_knomial_init_common(task);
out:
    return status;
}

// Placeholder functions for the undefined functions in the original code
ucc_status_t ucc_tl_ucp_allreduce_knomial_init_common(ucc_tl_ucp_task_t *task) {
    // Implement the common initialization logic for knomial allreduce here
    return UCC_OK;
}

ucc_tl_ucp_task_t *ucc_tl_ucp_init_task(ucc_base_coll_args_t *coll_args, ucc_base_team_t *team) {
    // Allocate and initialize the task structure
    ucc_tl_ucp_task_t *task = malloc(sizeof(ucc_tl_ucp_task_t));
    if (!task) {
        return NULL;
    }
    // Initialize the task fields here
    return task;
}
