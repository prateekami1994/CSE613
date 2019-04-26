#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<time.h>
#define msize 4096
#include<cilk/cilk.h>
int main() {

        int i, j, k, sum = 0;
        int a[msize][msize], b[msize][msize], res[msize][msize] = {{0}};
	for (i = 0; i < msize; i++ ) {
                for (j = 0; j < msize; j++) {
                        a[i][j] = rand() % INT_MAX;
                        b[i][j] = rand() % INT_MAX;
                }
        }
	time_t start = time(NULL);
	cilk_for ( int i = 0; i < msize; i++) {
		for ( k = 0; k < msize; k++) {
			cilk_for ( j = 0; j < msize; j++) {
				res[i][j] = res[i][j]+ (a[i][k] * b[k][j]);
			}

		}
	}
        printf("Time taken for n = %d is : %.2f\n", msize, (double)(time(NULL) - start));
	return 0;
}
