#ifndef SUBSEEKDTW
#define SUBSEEKDTW

#define QUERY_LEN 250
#define REF_LEN 1250

typedef float value_t;

typedef struct search_res_t {
    value_t dist;
    int position;
} search_res_t;

search_res_t subseek_dtw(value_t x[QUERY_LEN], value_t y[REF_LEN], int m, int n);

#endif
