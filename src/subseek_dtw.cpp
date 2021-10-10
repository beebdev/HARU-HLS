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
    value_t cost[QUERY_LEN];
#pragma HLS ARRAY_PARTITION variable = cost complete dim = 1

    search_result_t min;
    min.dist = VALUE_INF;
    min.end_position = 0;

cost_init_loop:
    /* Init cost column */
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

            // Update dependancy
            top_left = left;
            top = cost[r];
        }

        // Dependancy for new col
        top_left = 0;
        top = 0;

        // Update minimum
        if (cost[QUERY_LEN - 1] < min.dist) {
            min.dist = cost[QUERY_LEN - 1];
            min.end_position = c;
        }
    }

    return min;
}

/* Wrapper for the actual sDTW algorithm
 * Unpacks the streamed data from AXI stream into sequence x and y
 * Then calls the sDTW algorithm and transfers the data back */
template <typename T, int U, int TI, int TD>
void wrapped_sDTW(AXI_VAL in_stream[REF_LEN + QUERY_LEN], value_t *min_dist, int *min_pos,
                  value_t *val) {
    /* Reference and query buffers */
    T reference[REF_LEN], query[QUERY_LEN];

unpack_reference:
    /* Stream in y sequence */
    for (int i = 0; i < REF_LEN; i++) {
#pragma HLS PIPELINE
        reference[i] = pop_stream<T, U, TI, TD>(in_stream[i]);
    }

    /* Stream in query */
unpack_query:
    for (int i = 0; i < QUERY_LEN; i++) {
#pragma HLS PIPELINE
        query[i] = pop_stream<T, U, TI, TD>(in_stream[i]);
    }

    /* Start sDTW */
    search_result_t res = sDTW(query, reference);

    /* Pass results to output variables */
    *min_dist = res.dist;
    *min_pos = res.end_position;
    *val = reference[0];
}

/* Top level design that will be synthesized into RTL */
void subseek_dtw(AXI_VAL INPUT_STREAM[REF_LEN + QUERY_LEN], value_t *min_dist, int *min_pos,
                 value_t *val) {
    /* Port IO interface */
#pragma HLS INTERFACE axis port = INPUT_STREAM
#pragma HLS INTERFACE s_axilite port = min_dist bundle = CONTROL_BUS
#pragma HLS INTERFACE s_axilite port = min_pos bundle = CONTROL_BUS
#pragma HLS INTERFACE s_axilite port = val bundle = CONTROL_BUS
#pragma HLS INTERFACE s_axilite port = return bundle = CONTROL_BUS

    /* Call wrapper for sDTW */
    wrapped_sDTW<value_t, 4, 5, 5>(INPUT_STREAM, min_dist, min_pos, val);
}
