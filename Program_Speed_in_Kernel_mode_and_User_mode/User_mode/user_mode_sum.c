// Compile using this command:
// gcc user_mode_sum.c -o user_mode_sum

#include <stdio.h>
#include <time.h>
int main() {
    clock_t start, end;
    volatile unsigned long long sum = 0;
    FILE *fp;
    start = clock();
    for (unsigned int i = 1; i <= 1000000; ++i) {
        sum += i;
    }
    end = clock();
    double time_taken = (double)(end - start) / CLOCKS_PER_SEC;
    printf("User Mode Sum: %llu\n", sum);
    printf("Time Taken (User Mode): %f seconds\n", time_taken);
    fp = fopen("/root/timecaptured", "a");
    if (fp != NULL) {
        fprintf(fp, "User Mode: Sum = %llu, Time = %.6f seconds\n", sum, time_taken);
        fclose(fp);
    } else {
        perror("Error writing to /root/timecaptured");
    }

    return 0;
}