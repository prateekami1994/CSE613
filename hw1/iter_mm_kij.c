#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#define msize 4096
#include<time.h>
int main() {

        int i, j, k, sum = 0;
        int a[msize][msize], b[msize][msize], res[msize][msize] = {{0}};
        clock_t start, end;
 	double cpu_time_used;
	for (i = 0; i < msize; i++ ) {
                for (j = 0; j < msize; j++) {
                        a[i][j] = rand() % INT_MAX;
                        b[i][j] = rand() % INT_MAX;
                }
        }
	start = clock();
	for ( k = 0; k < msize; k++) {
		for ( i = 0; i < msize; i++) {
			for ( j = 0; j < msize; j++) {
				res[i][j] = res[i][j] + (a[i][k] * b[k][j]);
			}
		}
	}
	end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("CPU time used : %f\n", cpu_time_used);
	return 0;
}
