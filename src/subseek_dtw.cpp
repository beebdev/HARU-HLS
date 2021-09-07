#include <stdio.h>
#include <math.h>
#include "../include/subseek_dtw.h"
// #include <ap_axi_sdata.h>


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

search_result_t sDTW(value_t x[QUERY_LEN], value_t y[REF_LEN]) {
    // TODO: apply suitable directives for x and y buffers

    value_t cost[QUERY_LEN];
    #pragma HLS ARRAY_PARTITION variable=cost complete dim=1

    search_result_t min;
    min.dist = VALUE_INF;
    min.end_position = 0;

    /* Init cost column */
    cost_init_loop: for (int r = 0; r < QUERY_LEN; r++) {
        #pragma HLS UNROLL
        cost[r] = VALUE_INF;
    }

    /* Compute cost matrix */
    value_t top_left = 0;
    value_t top = 0;
    value_t left;

    seq_y_loop: for (int c = 0; c < REF_LEN; c++) {
        #pragma HLS pipeline II=1
        seq_x_loop: for (int r = 0; r < QUERY_LEN; r++) {
            left = cost[r];
            cost[r] = abs(x[r] - y[c]) + min3(top, left, top_left);
            
            // Update dependancy
            top_left = left;
            top = cost[r];
        }
    
        // Dependancy for new col
        top_left = 0;
        top = 0;

        // Update minimum
        if (cost[QUERY_LEN-1] < min_dist) {
            min.dist = cost[QUERY_LEN-1];
            min.end_position = c;
        }
    }

    return res;
}


/* Wrapper for the actual sDTW algorithm
 * Unpacks the streamed data from AXI stream into sequence x and y
 * Then calls the sDTW algorithm and transfers the data back */
// typedef ap_axiu<32, 4, 5, 5> AXI_VAL;
template <typename T, int U, int TI, int TD>
void wrapped_sDTW(AXI_VAL in_stream[QUERY_LEN + REF_LEN], value_t *min_dist, int *min_pos) {
    T query[QUERY_LEN], reference[REF_LEN];

    /* Stream in x sequence */
    for (int i = 0; i < Q_LEN; i++) {
        #pragma HLS PIPELINE
        query[i] = pop_stream<T, U, TI, TD>(in_stream[i]);
    }

    /* Stream in y sequence */
    for (int i = 0; i < R_LEN; i++) {
        #pragma HLS PIPELINE
        reference[i] = pop_stream<T, U, TI, TD>(in_stream[i]);
    }

    /* Start sDTW */
    search_result_t res = sDTW(query, reference);

    /* Pass results to output variables */
    *min_dist = res.dist;
    *min_pos = res.end_position;
}


/* Top level design that will be synthesized into RTL */
void subseek_dtw(AXI_VAL INPUT_STREAM[QUERY_LEN + REF_LEN], value_t *min_dist, int *min_pos) {
    /* Port IO interface */
    #pragma HLS INTERFACE axis port=INPUT_STREAM
    #pragma HLS INTERFACE s_axilite port=min_dist bundle=CONTROL_BUSS
    #pragma HLS INTERFACE s_axilite port=min_pos bundle=CONTROL_BUSS
    #pragma HLS INTERFACE s_axilite port=return bundle=CONTROL_BUS

    /* Call wrapper for sDTW */
    // TODO: update the template values
    wrapped_sDTW<value_t, 4, 5, 5>(INPUT_STREAM, min_dist, min_pos);
}