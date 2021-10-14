#include <stdio.h>
#include "subseek_dtw.h"

template <int U, int TI, int TD>
void wrapped_loader(AXI_VAL ref_stream[REF_LEN], seqval_t reference[REF_LEN]) {
	/* Stream in reference */
unpack_reference:
	for (int i = 0; i < REF_LEN; i++) {
#pragma HLS PIPELINE
		reference[i] = pop_stream<seqval_t, U, TI, TD>(ref_stream[i]);
	}
}

void reference_loader(AXI_VAL ref_stream[REF_LEN], seqval_t reference[REF_LEN]) {
#pragma HLS INTERFACE axis port = ref_stream
#pragma HLS INTERFACE bram port = reference
#pragma HLS INTERFACE s_axilite port = return bundle = CONTROL_BUS
	wrapped_loader<4, 5, 5>(ref_stream, reference);
	return;
}