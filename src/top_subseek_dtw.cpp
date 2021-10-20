#include <stdio.h>
#include <stdint.h>
#include "subseek_dtw.h"
#include "reference_int.h"

int main() {
	search_result_t expected_f, expected_r;
	expected_f = sDTW<int16_t, uint16_t>(strand_sample, reference[0]);
	expected_r = sDTW<int16_t, uint16_t>(strand_sample, reference[1]);

	printf("F: @%d [%d]\n", expected_f.pos, expected_f.dist);
	printf("R: @%d [%d]\n", expected_r.pos, expected_r.dist);
	return 0;
}
