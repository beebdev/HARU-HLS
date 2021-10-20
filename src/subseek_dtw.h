#ifndef SQUIGGLE_SEARCH
#define SQUIGGLE_SEARCH

#include <ap_axi_sdata.h>
//#include "ap_int.h"
 #include <stdint.h>

#define SEQ_BIT_WIDTH 16
#define COST_BIT_WIDTH 18
#define VALUE_INF (1 << (COST_BIT_WIDTH - 1)) - 1
#define QUERY_LEN 250
#define REF_LEN 48497


/* Data type definitions */
// typedef ap_uint<COST_BIT_WIDTH> value_t;
// typedef ap_int<SEQ_BIT_WIDTH> seqval_t;
typedef uint16_t value_t; // Used for cost array
typedef int16_t seqval_t; // Used for reference and query

/* Type for query AXI Stream */
typedef ap_axis<sizeof(seqval_t) * 8, 4, 5, 5> AXI_VAL;

/* Result type, used for multi output ports */
typedef struct search_result_t {
	bool ID;
	unsigned int pos;
	unsigned int dist;
	// int x;
	// int y;
} search_result_t;

/* Subsequence DTW Implementation */
template <typename valT>
valT min3(valT a, valT b, valT c) {
	valT min = a;
	if (b < min) {
		min = b;
	}
	if (c < min) {
		min = c;
	}
	return min;
}

template <typename seqT, typename valT>
search_result_t sDTW(seqT x[QUERY_LEN], seqT y[REF_LEN], bool ID) {

	/* Cost array - completely partitioned */
	valT cost[QUERY_LEN];
#pragma HLS ARRAY_PARTITION variable = cost complete dim = 1

	search_result_t min;
	min.ID = ID;
	min.dist = VALUE_INF;
	min.pos = 0;
	// min.x = x[0];
	// min.y = y[0];

	/* Init cost column */
cost_init_loop:
	for (int r = 0; r < QUERY_LEN; r++) {
#pragma HLS UNROLL
		cost[r] = VALUE_INF;
	}

	/* Compute cost matrix */
	valT top_left = 0;
	valT top = 0;
	valT left;

seq_y_loop:
	for (int c = 0; c < REF_LEN; c++) {
#pragma HLS pipeline II = 1
	seq_x_loop:
		for (int r = 0; r < QUERY_LEN; r++) {
			left = cost[r];
			cost[r] = (valT)abs(x[r] - y[c]) + min3<valT>(top, left, top_left);

			/* Update dependancy */
			top_left = left;
			top = cost[r];
		}

		/* Dependancy for next col */
		top_left = 0;
		top = 0;

		/* Update minimum */
		if (cost[QUERY_LEN - 1] < min.dist) {
			min.pos = c;
			min.dist = cost[QUERY_LEN - 1];
		}
	}

	return min;
}

/* Function Prototypes */
search_result_t subseek_dtw(AXI_VAL query[QUERY_LEN]);
void reference_loader(AXI_VAL ref_stream[REF_LEN], seqval_t reference[REF_LEN]);

/* Pop functions for AXI Stream */
template <typename T, int U, int TI, int TD>
T pop_stream(ap_axis<sizeof(T) * 8, U, TI, TD> const &e) {
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
