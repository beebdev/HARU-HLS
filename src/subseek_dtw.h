#ifndef SQUIGGLE_SEARCH
#define SQUIGGLE_SEARCH

#include <ap_axi_sdata.h>
#include "ap_int.h"
#include <stdint.h>

#define BIT_WIDTH 16
#define VALUE_INF (1 << (BIT_WIDTH - 1)) - 1
#define QUERY_LEN 250
#define REF_LEN 48497

typedef ap_int<BIT_WIDTH> value_t;
// typedef int16_t value_t;

typedef ap_axiu<sizeof(value_t) * 8, 4, 5, 5> AXI_VAL;

typedef struct search_result_t {
    value_t dist;
    int end_position;
} search_result_t;

/* Reference Buffer */
value_t reference[REF_LEN];

/* Function Prototypes */
void subseek_dtw(AXI_VAL INPUT_STREAM[REF_LEN], value_t *min_dist, int *min_pos, value_t *val);
search_result_t sDTW(value_t x[QUERY_LEN], value_t y[REF_LEN]);
// search_result_t standalone_sDTW(value_t x[QUERY_LEN], value_t y[REF_LEN]);

/* Push and pop functions for AXI Stream */
template <typename T, int U, int TI, int TD>
T pop_stream(ap_axiu<sizeof(T) * 8, U, TI, TD> const &e) {
#pragma HLS INLINE
    T ret = e.data;
    volatile ap_uint<sizeof(T)> strb = e.strb;
    volatile ap_uint<sizeof(T)> keep = e.keep;
    volatile ap_uint<U> user = e.user;
    volatile ap_uint<1> last = e.last;
    volatile ap_uint<TI> id = e.id;
    volatile ap_uint<TD> dest = e.dest;
    return ret;
}

// template <typename T, int U, int TI, int TD>
// AXI_VAL push_stream(T const &v, bool last = false) {
// #pragma HLS INLINE
//     AXI_VAL e;
//     e.data = v;
//     e.strb = (1 << sizeof(T)) - 1;
//     e.keep = (1 << sizeof(T)) - 1;
//     e.user = 0;
//     e.last = last ? 1 : 0;
//     e.id = 0;
//     e.dest = 0;
//     return e;
// }

#endif
