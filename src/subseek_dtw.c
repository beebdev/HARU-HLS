#include <stdio.h>
#include <math.h>
#include "../include/subseek_dtw.h"

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

search_result_t subseek_dtw(value_t x[QUERY_LEN], value_t y[REF_LEN]) {
    value_t cost[QUERY_LEN];
    search_result_t res;
    res.dist = VALUE_INF;
    res.end_position = 0;

    /* Init cost column */
    cost_init_loop: for (int r = 0; r < QUERY_LEN; r++) {
        cost[r] = VALUE_INF;
    }

    /* Compute cost matrix */
    value_t top_left = 0;
    value_t top = 0;
    value_t left;

    seq_y_loop: for (int c = 0; c < REF_LEN; c++) {
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
        if (cost[QUERY_LEN-1] < res.dist) {
            res.dist = cost[QUERY_LEN-1];
            res.end_position = c;
        }
    }

    return res;
}
