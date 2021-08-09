#include <math.h>
#include "subseek_dtw.h"

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

int cost_index_rotate(int index) {
    index -= 1;
    if (index == -1) {
        index = 2;
    }
    return index;
}

search_res_t subseek_dtw(value_t x[QUERY_LEN], value_t y[REF_LEN], int m, int n) {
    /* x is the query sequence with length QUERY_LEN
     * y is the reference sequence with length REF_LEN
     */

    // Cost matrix that stores only previous two rows and current row
    value_t cost[3][QUERY_LEN];
    int curr = 0, p1 = 1, p2 = 2;

    // First QUERY_LEN sequence
    int curr_y;
    cost[p2][0] = fabs(x[0] - y[0]);
    cost[p1][0] = cost[2][0];
    cost[p1][1] = fabs(x[1] - y[0]) + cost[2][0];
    for (curr_y = 2; curr_y < m; curr_y++) {
        // First curr cell, no top and top-left
        cost[curr][0] = cost[p1][0];

        // All in between curr cells
        for (int c = 1; c < curr_y; c++) {
            cost[curr][c] = fabs(x[c] - y[curr_y-c]) + min3(cost[p1][c-1], cost[p1][c], cost[p2][c-1]);
        }

        // Last curr cell, no left
        cost[curr][curr_y] = cost[p1][curr_y-1];

        // Rotate index
        curr = cost_index_rotate(curr);
        p1 = cost_index_rotate(p1);
        p2 = cost_index_rotate(p2); 
    }

    // Full lenghts of prevs and curr cost
    search_res_t min;
    min.dist = cost[p1][m-1];
    min.position = 0;
    for (curr_y = m; curr_y < n; curr_y++) {
        // First curr cell, no top and top-left
        cost[curr][0] = cost[p1][0];

        // The rest of curr cells
        for (int c = 1; c < m; c++) {
            cost[curr][c] = fabs(x[c] - y[curr_y-c]) + min3(cost[p1][c-1], cost[p1][c], cost[p2][c-1]);
        }

        // Update the minimum
        if (cost[curr][m-1] < min.dist) {
            min.dist = cost[curr][m-1];
            min.position = curr_y;
        }

        // Rotate index
        curr = cost_index_rotate(curr);
        p1 = cost_index_rotate(p1);
        p2 = cost_index_rotate(p2); 
    }

    // Last QUERY_LEN sequence
    for (int curr_x = 1; curr_x < m; curr_x++) {
        for (int c = 0; c < m-curr_x; c++) {
            cost[curr][c] = fabs(x[curr_x + c] - y[n-1 - c]) + min3(cost[p1][c], cost[p1][c+1], cost[p2][c]);
        }
        
        // Update minimum
        if (cost[curr][m-curr_x-1] < min.dist) {
            min.dist = cost[curr][m-curr_x -1];
            min.position = n - (m-curr_x);
        }

        // Rotate index
        curr = cost_index_rotate(curr);
        p1 = cost_index_rotate(p1);
        p2 = cost_index_rotate(p2); 
    }

    return min;
}