#include <stdio.h>
#include <sys/time.h>
#include "subseek_dtw.h"
#include "reference.h"

int main () {
    struct timeval t1, t2;
    gettimeofday(&t1, NULL);
    search_res_t res = subseek_dtw(sample_0, ref_signal[0][0], QUERY_LEN, REF_LEN);
    gettimeofday(&t2, NULL);
    double total_duration = (t2.tv_sec - t1.tv_sec) * 1000;
    total_duration += (t2.tv_usec - t1.tv_usec) / 1000;
    printf("Time: %lfms\n", total_duration);
    printf("Distance: %f\n", res.dist);
    printf("Position: %d\n", res.position);

    res = subseek_dtw(sample_0, ref_signal[0][1], QUERY_LEN, REF_LEN);
    printf("Distance: %f\n", res.dist);
    printf("Position: %d\n", res.position);
    return 0;
}