#include <bits/stdc++.h>
#include <cilk/cilk.h>
#include <chrono>
#include <cilk/cilk_api.h>

using namespace std;
using namespace std::chrono;


int * prefix_sum(int* in , int n){
	int *sum = new int[n];

	if(n==1){
		sum[0] = in[0];
	}else{
		int *z ,*y = new int [n/2];

		cilk_for(int i=0;i<n/2;i++){
			y[i]=in[2*i]+in[2*i+1];			
		}

		z = prefix_sum(y,n/2);
		sum[0]=in[0];

		cilk_for(int i=1;i<n;i++){
			if(i%2 != 0){
				sum[i]=z[i/2];
			}else{
				sum[i]=z[(i-1)/2] + in[i];
			}
		}
	}
	return sum;
}

int par_partition(int*a, int q, int r, int x){
	int n = r-q+1;
	if (n==1){
		return q;
	}

	int * b = new int[n];
	int *lt = new int[n];
	int *gt = new int[n];
	int k;
	int *lt_ps, *gt_ps;
	
	cilk_for (int i=0;i<=n-1;i++){
		b[i] = a[q+i];
		if(b[i]<x){
			lt[i] = 1;
		}else{
			lt[i] = 0;
		}
		if(b[i]>x){
			gt[i] = 1;
		}else{
			gt[i] = 0;
		}
	}
	lt_ps = prefix_sum(lt,n);
	gt_ps = prefix_sum(gt,n);
	k = q + lt_ps[n-1];
	a[k]=x;

	cilk_for(int i=0;i<=n-1;i++){
		if(b[i]<x){
			a[q+lt_ps[i]-1] = b[i];
		}else if(b[i]>x){
			a[k+gt_ps[i]]=b[i];
		}
	}
	return k;
}


void par_randomized_quicksort(int* a, int q, int r){
	int n = r-q+1;

	if (n<=1048576){
		sort(a+q,a+r+1);
	}
	else{
		int min,max;
		min = q;
		max = r;
		srand (time(NULL));
		int index = min + (rand() % (max-min+1));
	
		int x = a [index];
		int k = par_partition(a,q,r,x);
		cilk_spawn par_randomized_quicksort(a,q,k-1);
		par_randomized_quicksort(a,k+1,r);
		cilk_sync;
	}
}

void question1cPart1(){

	__cilkrts_set_param("nworkers","1");
        int workers = __cilkrts_get_nworkers();
        cout<<"Number of Workers: "<<workers<<endl;

	int n=pow(2,27);
	int * a = new int [n];
	int q=0, r=n-1;
	
	for(int i =0;i<n;i++){
		a[i] = n - i;
	}

	auto start = high_resolution_clock::now();
	par_randomized_quicksort(a,0,n-1);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<seconds>(stop - start);

	cout<<endl;
	cout <<"Time taken to sort : "<<duration.count() << endl;
}

int main()
{
	question1cPart1();
	return 0;
}
