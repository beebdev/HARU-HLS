#include "subseek_dtw.h"

#include <math.h>
#include <stdio.h>

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
search_result_t sDTW(value_t x[QUERY_LEN], value_t y[REF_LEN]) {

	/* Cost array - completely partitioned */
	value_t cost[QUERY_LEN];
#pragma HLS ARRAY_PARTITION variable = cost complete dim = 1

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
#pragma HLS pipeline II = 1
	seq_x_loop:
		for (int r = 0; r < QUERY_LEN; r++) {
			left = cost[r];
			cost[r] = abs(x[r] - y[c]) + min3(top, left, top_left);

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
template <typename T, int U, int TI, int TD>
search_result_t wrapped_sDTW(AXI_VAL query_in[QUERY_LEN], AXI_VAL reference_in[REF_LEN], int load_ref) {

	search_result_t res;

	T reference[REF_LEN];
#pragma HLS RESOURCE variable = reference core = RAM_2P_BRAM

	if (load_ref) {
		/* Stream in reference */
	unpack_ref:
		for (int i = 0; i < REF_LEN; i++) {
#pragma HLS PIPELINE
			reference[i] = (value_t)pop_stream<stream_t, U, TI, TD>(reference_in[i]);
		}
		res.dist = 0;
		res.pos = 0;
	} else {
		/* Stream in query */
		T query[QUERY_LEN];
	unpack_query:
		for (int i = 0; i < QUERY_LEN; i++) {
#pragma HLS PIPELINE
			query[i] = (value_t)pop_stream<stream_t, U, TI, TD>(query_in[i]);
		}
		/* Start sDTW */
		res = sDTW(query, reference);
	}
	return res;
}

/* Top level design that will be synthesized into RTL */
search_result_t subseek_dtw(AXI_VAL query[QUERY_LEN], AXI_VAL reference[REF_LEN], int opcode) {
	/* Port IO interface */
#pragma HLS INTERFACE axis port = query
#pragma HLS INTERFACE axis port = reference
#pragma HLS INTERFACE s_axilite port = opcode bundle = CONTROL_BUS
#pragma HLS INTERFACE s_axilite port = return bundle = CONTROL_BUS

	/* Call wrapper for sDTW */
	return wrapped_sDTW<value_t, 4, 5, 5>(query, reference, opcode);
}
