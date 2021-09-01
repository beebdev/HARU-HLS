#ifndef SQUIGGLE_SEARCH
#define SQUIGGLE_SEARCH

#include "ap_cint.h"

#define BLOCKSIZE 200000
#define OVERLAP (BLOCKSIZE-500)
#define START 10000
#define END 15000
#define LIBLENGTH 3000
#define BALANCE (LIBLENGTH/2)
#define NREF 1
#define QUERY_LEN 250
#define REF_LEN 48497
// #define REF_LEN 4000

#define ISFLOAT 0

typedef int18 value_t;
#define VALUE_INF (1 << 17) - 1

typedef struct search_result_t {
    value_t dist;
    int end_position;
} search_result_t;

search_result_t subseek_dtw(value_t query[QUERY_LEN], value_t ref[REF_LEN]);

#endif
