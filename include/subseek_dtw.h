
#define QUERY_LEN 250
#define REF_LEN 1000

typedef float value_t;

typedef struct search_res_t {
    value_t dist;
    int position;
} search_res_t;