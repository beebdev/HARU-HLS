#include <stdio.h>
#include "subseek_dtw.h"
#include "reference_int.h"

int main() {
    search_result_t expected_f, expected_r;
    expected_f = sDTW(strand_sample, reference_eg[0]);
    expected_r = sDTW(strand_sample, reference_eg[1]);

    printf("F: @%d [%d]\n", expected_f.end_position, expected_f.dist);
    printf("R: @%d [%d]\n", expected_r.end_position, expected_r.dist);
    return 0;
}
