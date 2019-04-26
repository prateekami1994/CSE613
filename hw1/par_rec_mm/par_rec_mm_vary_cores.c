#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#define M 13
#define N (1<<M)
int X[N+1][N+1];
int Y[N+1][N+1];
int Z[N+1][N+1];

void set_rand() {
        for (int i = 1; i <= N; i++) {
                for (int j = 1; j <= N; j++) {
                	X[i][j] = 1;//rand() % 10;
			Y[i][j] = 1;
			Z[i][j] = 0;
                }
        }
}

void printMatrix() {
         for (int i = 1; i <= N; i++) {
                for (int j = 1; j <= N; j++) {
                        printf("%d ", Z[i][j]);
                }
                printf("\n");
        }
}

void rec_mm (int ztopleftX, int ztopleftY, int xtopleftX, int xtopleftY, int ytopleftX, int ytopleftY, int size, int base){
	if (size <= base){
		for ( int i = 0; i < size; i++) {
                	for ( int k = 0; k < size; k++) {
                        	for ( int j = 0; j < size; j++) {
                                	Z[ztopleftX+i][ztopleftY+j] = Z[ztopleftX+i][ztopleftY+j]+ X[xtopleftX+i][xtopleftY+k] * Y[ytopleftX+k][ytopleftY+j];
                        	}
                	}
		}	
	} else {
		cilk_spawn rec_mm(ztopleftX, ztopleftY, xtopleftX, xtopleftY, ytopleftX, ytopleftY, size/2, base);
		cilk_spawn rec_mm(ztopleftX, ztopleftY+size/2, xtopleftX, xtopleftY, ytopleftX, ytopleftY+size/2,size/2, base);
		cilk_spawn rec_mm(ztopleftX+size/2, ztopleftY, xtopleftX+size/2, xtopleftY, ytopleftX, ytopleftY,size/2, base);
		rec_mm(ztopleftX+size/2, ztopleftY+size/2, xtopleftX+size/2, xtopleftY, ytopleftX, ytopleftY+size/2,size/2, base);
		cilk_sync;
		cilk_spawn rec_mm(ztopleftX, ztopleftY, xtopleftX, xtopleftY+size/2, ytopleftX+size/2, ytopleftY,size/2, base);
		cilk_spawn rec_mm(ztopleftX, ztopleftY+size/2, xtopleftX, xtopleftY+size/2, ytopleftX+size/2, ytopleftY+size/2,size/2, base);
		cilk_spawn rec_mm(ztopleftX+size/2, ztopleftY, xtopleftX+size/2, xtopleftY+size/2, ytopleftX+size/2, ytopleftY,size/2, base);
		rec_mm(ztopleftX+size/2, ztopleftY+size/2, xtopleftX+size/2, xtopleftY+size/2, ytopleftX+size/2, ytopleftY+size/2,size/2, base);
		cilk_sync;
	}
}

int main(int argc, char* argv[]){
	clock_t start, end;
  	double cpu_time_used;
	set_rand();
	__cilkrts_set_param("nworkers", argv[1]);
	start = clock();	
	rec_mm(1,1,1,1,1,1,N,1024);
	end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("CPU time used for nworkers=%d for base case of 256 is : %f\n", __cilkrts_get_nworkers(), cpu_time_used);
	return 0;
}
