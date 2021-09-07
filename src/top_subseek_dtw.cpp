#include <stdio.h>
#include <sys/time.h>
#include "../include/subseek_dtw.h"
#include "../include/reference_int.h"


int main() {
    struct timeval t1, t2;
    
    gettimeofday(&t1, NULL);
    search_result_t res = subseek_dtw(strand_sample, reference[0]);
    gettimeofday(&t2, NULL);

    double total_duration = (t2.tv_sec - t1.tv_sec) * 1000;
    total_duration += (t2.tv_usec - t1.tv_usec) / 1000;
    printf("Time: %lfms\n", total_duration);
    printf("Distance: %d\n", res.dist);
    printf("Position: %d\n", res.end_position);

    res = subseek_dtw(strand_sample, reference[1]);
    printf("Distance: %d\n", res.dist);
    printf("Position: %d\n", res.end_position);
    return 0;
}
