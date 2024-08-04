#ifndef CONFIG_H
#define CONFIG_H

#define UCS_HAVE_PARSER_SET_VALUE_TABLE_PREFIX

// Define Allreduce Algorithms
#define UCC_TL_UCP_ALLREDUCE_ALG_KNOMIAL       0
#define UCC_TL_UCP_ALLREDUCE_ALG_SRA_KNOMIAL   1
#define UCC_TL_UCP_ALLREDUCE_ALG_DBT           2
#define UCC_TL_UCP_ALLREDUCE_ALG_SLIDING_WINDOW 3
#define UCC_TL_UCP_ALLREDUCE_ALG_LAST          4

// Define other configuration parameters
#define MAX_MSG_SIZE (10 * 1024 * 1024) // 10 MB
#define DEFAULT_ALG UCC_TL_UCP_ALLREDUCE_ALG_KNOMIAL

// Feature flags
#define ENABLE_DEBUG 1

#endif // CONFIG_H
