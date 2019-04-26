#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#define msize 4096
#include<papi.h>
#include<time.h>
#define ERROR_RETURN(retval) { fprintf(stderr, "Error %d %s:line %d: \n", retval,__FILE__,__LINE__);  exit(retval); }

int main() {
	int numEvents = 1;
   	long long values[2];
    	int events[1] = {PAPI_L2_TCM};
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
 	if (PAPI_start_counters(events, numEvents) != PAPI_OK)
		ERROR_RETURN(1);
	  
	for ( j = 0; j < msize; j++) {
		for ( k = 0; k < msize; k++) {
			for ( i = 0; i < msize; i++) {
				res[i][j] = res[i][j] + a[i][k] * b[k][j];
			}
			//res[i][j] = sum;
			//sum = 0;
		}
	}
  	if ( PAPI_stop_counters(values, numEvents) != PAPI_OK)
       		 ERROR_RETURN(1);
	end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("CPU time used : %f\n", cpu_time_used);
	printf("L1 Cache miss: %lld\n", values[0]);
	//printf("L2 Cache miss: %lld\n", values[1]);
	return 0;
}
