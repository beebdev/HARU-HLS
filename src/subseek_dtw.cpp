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

value_t *cost_ptr_rotate(value_t *ptr, value_t *cost_0, value_t *cost_1, value_t *cost_2) {
    if (ptr == cost_0) {
        ptr = cost_2;
    } else if (ptr == cost_1) {
        ptr = cost_0;
    } else {
        ptr = cost_1;
    }
    return ptr;
}

search_res_t subseek_dtw(value_t x[QUERY_LEN], value_t y[REF_LEN], int m, int n) {
    /* x is the query sequence with length QUERY_LEN
     * y is the reference sequence with length REF_LEN
     */

    // Cost matrix that stores only previous two rows and current row
    value_t cost_0[QUERY_LEN];
    value_t cost_1[QUERY_LEN];
    value_t cost_2[QUERY_LEN];
    // int curr = 0, p1 = 1, p2 = 2;
    value_t *curr = cost_0;
    value_t *p1 = cost_1;
    value_t *p2 = cost_2;

    // First QUERY_LEN sequence
    int curr_y;
    p2[0] = fabs(x[0] - y[0]);
    p1[0] = fabs(x[0] - y[1]);
    p1[1] = fabs(x[1] - y[0]) + p2[0];
    head_loop_1: for (curr_y = 2; curr_y < QUERY_LEN; curr_y++) {
        // First curr cell, no top and top-left
        curr[0] = fabs(x[0]- y[curr_y]);

        // All in between curr cells
        head_loop_2: for (int c = 1; c < curr_y; c++) {
            curr[c] = fabs(x[c] - y[curr_y-c]) + min3(p1[c-1], p1[c], p2[c-1]);
        }

        // Last curr cell, no left
        curr[curr_y] = fabs(x[curr_y] - y[0]) + p1[curr_y-1];

        // Rotate index
        curr = cost_ptr_rotate(curr, cost_0, cost_1, cost_2);
        p1 = cost_ptr_rotate(p1, cost_0, cost_1, cost_2);
        p2 = cost_ptr_rotate(p2, cost_0, cost_1, cost_2); 
    }

    // Full lenghts of prevs and curr cost
    search_res_t min;
    min.dist = p1[m-1];
    min.position = 0;
    body_loop_1: for (curr_y = QUERY_LEN; curr_y < REF_LEN; curr_y++) {
        // First curr cell, no top and top-left
        curr[0] = fabs(x[0] - y[curr_y]);

        // The rest of curr cells
        body_loop_2: for (int c = 1; c < QUERY_LEN; c++) {
            curr[c] = fabs(x[c] - y[curr_y-c]) + min3(p1[c-1], p1[c], p2[c-1]);
        }

        // Update the minimum
        if (curr[m-1] < min.dist) {
            min.dist = curr[m-1];
            min.position = curr_y - m + 1;
        }

        // Rotate index
        curr = cost_ptr_rotate(curr, cost_0, cost_1, cost_2);
        p1 = cost_ptr_rotate(p1, cost_0, cost_1, cost_2);
        p2 = cost_ptr_rotate(p2, cost_0, cost_1, cost_2); 
    }

    tail_loop_1: for (int curr_x = 1; curr_x < QUERY_LEN; curr_x++) {
    	tail_loop_2: for (int c = curr_x; c < QUERY_LEN; c++) {
            curr[c] = fabs(x[c] - y[n-curr_x]) + min3(p1[c-1], p1[c], p2[c-1]);
        }

        // Update the minimum
        if (curr[m-1] < min.dist) {
            min.dist = curr[m-1];
            min.position = curr_y - m + 1;
        }

        // Rotate index
        curr = cost_ptr_rotate(curr, cost_0, cost_1, cost_2);
        p1 = cost_ptr_rotate(p1, cost_0, cost_1, cost_2);
        p2 = cost_ptr_rotate(p2, cost_0, cost_1, cost_2); 
    }

    return min;
}
