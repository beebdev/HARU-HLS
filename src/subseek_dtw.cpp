#include <math.h>
#include <stdio.h>
#include "subseek_dtw.h"

#include "reference_int.h"

/* Wrapper for the sDTW core function.
 * Unpacks the AXI stream data to a query array and calls the core accelerator*/
template <int U, int TI, int TD>
search_result_t wrapped_sDTW(AXI_VAL query_in[QUERY_LEN], seqval_t f_ref[REF_LEN], seqval_t r_ref[REF_LEN]) {

	/* Stream in query */
	seqval_t query_0[QUERY_LEN];
//	seqval_t query_1[QUERY_LEN];
unpack_query:
	for (int i = 0; i < QUERY_LEN; i++) {
#pragma HLS PIPELINE
		query_0[i] = pop_stream<seqval_t, U, TI, TD>(query_in[i]);
//		query_1[i] = query_0[i];
	}

	/* Start sDTW */
//#pragma HLS DATAFLOW
	search_result_t r_result = sDTW<seqval_t, value_t>(query_0, r_ref, 1);
	search_result_t f_result = sDTW<seqval_t, value_t>(query_0, f_ref, 0);
	if (r_result.dist < f_result.dist) {
		return r_result;
	}
	return f_result;
}

/* Top level design that will be synthesized into RTL */
search_result_t subseek_dtw(AXI_VAL query[QUERY_LEN]/*, seqval_t reference[REF_LEN]*/) {
	/* Port IO interface */
#pragma HLS INTERFACE axis port = query
#pragma HLS INTERFACE s_axilite port = return bundle = CONTROL_BUS

	/* Call wrapper for sDTW */
	return wrapped_sDTW<4, 5, 5>(query, reference[0], reference[1]);
}
