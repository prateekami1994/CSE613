#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<time.h>
#define msize 4096
#include<cilk/cilk.h>
#include <papi.h>

#define ERROR_RETURN(retval) {fprintf(stderr, "Error %d %s: line %d: \n", retval, __FILE__, __LINE__); exit(retval); }

int main() {
	int numEvents = 1;
	long long values[2];
	int events[1] = {PAPI_L2_TCM};
        int a[msize][msize], b[msize][msize], res[msize][msize] = {{0}};
	for (int i = 0; i < msize; i++ ) {
                for (int j = 0; j < msize; j++) {
                        a[i][j] = rand() % INT_MAX;
                        b[i][j] = rand() % INT_MAX;
                }
        }
	time_t start = time(NULL);
	if (PAPI_start_counters(events, numEvents) != PAPI_OK)
		ERROR_RETURN(1)
	cilk_for ( int i = 0; i < msize; i++) {
		cilk_for ( int k = 0; k < msize; k++) {
			for ( int j = 0; j < msize; j++) {
				res[i][j] = res[i][j]+ (a[i][k] * b[k][j]);
			}
		}
	}
	if (PAPI_stop_counters(values, numEvents) != PAPI_OK)
		ERROR_RETURN(1)
        printf("Time taken for n=%d is : %.2f\n", msize, (double)(time(NULL) - start));
	printf("L1 Cache miss: %lld\n", values[0]);
	return 0;
}
