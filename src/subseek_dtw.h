#ifndef SQUIGGLE_SEARCH
#define SQUIGGLE_SEARCH

#include <ap_axi_sdata.h>
#include "ap_int.h"
// #include <stdint.h>

#define SEQ_BIT_WIDTH 16
#define COST_BIT_WIDTH 18
#define VALUE_INF (1 << (COST_BIT_WIDTH - 1)) - 1
#define QUERY_LEN 250
#define REF_LEN 48497

typedef ap_int<COST_BIT_WIDTH> value_t;
typedef ap_int<SEQ_BIT_WIDTH> seqval_t;

typedef ap_axiu<sizeof(seqval_t) * 8, 4, 5, 5> AXI_VAL;

typedef struct search_result_t {
	int pos;
	int dist;
} search_result_t;

/* Function Prototypes */
search_result_t subseek_dtw(AXI_VAL query[QUERY_LEN]);
search_result_t sDTW(seqval_t x[QUERY_LEN], seqval_t y[REF_LEN]);

/* Pop functions for AXI Stream */
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

#endif
