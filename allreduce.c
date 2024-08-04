#include "config.h"
#include "tl_ucp.h"
#include "allreduce.h"
#include "ucc_coll_utils.h"

// Implement Allreduce algorithm selection logic
switch (selected_algorithm) {
    case UCC_TL_UCP_ALLREDUCE_ALG_KNOMIAL:
        // Perform knomial Allreduce
        status = ucc_tl_ucp_allreduce_knomial_init(coll_args, team, task_h);
        if (status != UCC_OK) {
            fprintf(stderr, "Failed to initialize knomial Allreduce\n");
        }
        break;
        
    case UCC_TL_UCP_ALLREDUCE_ALG_SRA_KNOMIAL:
        // Perform SRA knomial Allreduce
        status = ucc_tl_ucp_allreduce_sra_knomial_init(coll_args, team, task_h);
        if (status != UCC_OK) {
            fprintf(stderr, "Failed to initialize SRA knomial Allreduce\n");
        }
        break;
        
    case UCC_TL_UCP_ALLREDUCE_ALG_DBT:
        // Perform DBT Allreduce
        status = ucc_tl_ucp_allreduce_dbt_init(coll_args, team, task_h);
        if (status != UCC_OK) {
            fprintf(stderr, "Failed to initialize DBT Allreduce\n");
        }
        break;
        
    case UCC_TL_UCP_ALLREDUCE_ALG_SLIDING_WINDOW:
        // Perform Sliding Window Allreduce
        status = ucc_tl_ucp_allreduce_sliding_window_init(coll_args, team, task_h);
        if (status != UCC_OK) {
            fprintf(stderr, "Failed to initialize Sliding Window Allreduce\n");
        }
        break;
        
    default:
        fprintf(stderr, "Invalid Allreduce algorithm selected\n");
        status = UCC_ERR_INVALID_PARAM;
        break;
}
