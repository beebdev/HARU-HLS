#ifndef SQUIGGLE_SEARCH
#define SQUIGGLE_SEARCH

#include "ap_cint.h"
#include <ap_axi_sdata.h>

typedef ap_axiu<32, 4, 5, 5> AXI_VAL;
typedef int16 value_t;
#define VALUE_INF (1 << 17) - 1
#define QUERY_LEN 250
#define REF_LEN 48497

typedef struct search_result_t {
    value_t dist;
    int end_position;
} search_result_t;

void subseek_dtw(AXI_VAL INPUT_STREAM[QUERY_LEN + REF_LEN], AXI_VAL OUTPUT_REG[1024]);

#endif
