

#include <math.h>
#include <stdio.h>
#include "subseek_dtw.h"
#include "reference_int.h"

/* Finds the minimum of the three parameter values */
value_t min3(value_t a, value_t b, value_t c) {
	value_t min = a;
	if (b < min) {
		min = b;
	}
	if (c < min) {
		min = c;
	}
	return min;
}

/* sDTW kernel */
search_result_t sDTW(seqval_t x[QUERY_LEN], seqval_t y[REF_LEN]) {

	/* Cost array - completely partitioned */
	value_t cost[QUERY_LEN];
#pragma HLS ARRAY_PARTITION variable=cost complete dim=1

	search_result_t min;
	min.dist = VALUE_INF;
	min.pos = 0;

	/* Init cost column */
cost_init_loop:
	for (int r = 0; r < QUERY_LEN; r++) {
#pragma HLS UNROLL
		cost[r] = VALUE_INF;
	}

	/* Compute cost matrix */
	value_t top_left = 0;
	value_t top = 0;
	value_t left;

seq_y_loop:
	for (int c = 0; c < REF_LEN; c++) {
#pragma HLS pipeline II=1
	seq_x_loop:
		for (int r = 0; r < QUERY_LEN; r++) {
			left = cost[r];
			cost[r] = (value_t)abs(x[r] - y[c]) + min3(top, left, top_left);

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

/* Wrapper for the actual sDTW algorithm
 * Unpacks the streamed data from AXI stream into sequence x and y
 * Then calls the sDTW algorithm and transfers the data back */
template <int U, int TI, int TD>
search_result_t wrapped_sDTW(AXI_VAL query_in[QUERY_LEN], seqval_t reference[REF_LEN]) {

	/* Stream in query */
	seqval_t query[QUERY_LEN];
unpack_query:
	for (int i = 0; i < QUERY_LEN; i++) {
#pragma HLS PIPELINE
		query[i] = pop_stream<seqval_t, U, TI, TD>(query_in[i]);
	}
	/* Start sDTW */
	return sDTW(query, reference);
}

/* Top level design that will be synthesized into RTL */
search_result_t subseek_dtw(AXI_VAL query[QUERY_LEN], seqval_t reference[REF_LEN]) {
	/* Port IO interface */
#pragma HLS INTERFACE axis port=query
#pragma HLS INTERFACE bram port=reference
#pragma HLS INTERFACE s_axilite port=return bundle=CONTROL_BUS

	/* Call wrapper for sDTW */
	return wrapped_sDTW<4, 5, 5>(query, reference);
}
