#include <stdio.h>

#include "../include/reference_int.h"
#include "../include/subseek_dtw.h"

int main() {
    search_result_t expected_f, expected_r;
    expected_f = sDTW(strand_sample, reference_eg[0]);
    expected_r = sDTW(strand_sample, reference_eg[1]);

    printf("F: @%d [%d]", expected_f.end_position, expected_f.dist);
    printf("R: @%d [%d]", expected_r.end_position, expected_r.dist);
    return 0;
}
