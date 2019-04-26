#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <math.h>
#include <algorithm>
#include <vector>
#include <cilk/cilk.h>
#include <iomanip>
#include <fstream>
#include <chrono>
#include <mutex>
#include <cilk/cilk_api.h>
using namespace std;
 
typedef unsigned long long ULL;
ULL SHIFT_LEFT(ULL x, ULL y){
    return x << y;
}

ULL SHIFT_RIGHT(ULL x, ULL y){
    return x >> y;
}

time_t seconds;

ULL* prefix_sum(ULL* in , int n){
    ULL* sum = new ULL[n];

    if(n==1){
        sum[0] = in[0];
    }else{
        ULL *z ,*y= new ULL [n/2];

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

void printMatrix(int** a, int n, int m){

    for(int i=0;i<n;i++){
        for(int j=0; j<m;j++){
            cout<<a[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}

void printArray(int* a, int n){
    for(int i=0;i<n;i++){
        cout<<a[i]<<" ";
    }
    cout<<endl;
}

void par_counting_rank(ULL f[] , int n, int r, ULL s[]){
    int p =68;
    int d = pow(2,r);

    ULL *js  = new ULL [p];
    ULL *je  = new ULL [p];
    ULL *ofs = new ULL [p];

    ULL** s1 = new ULL*[d];
    ULL** s2 = new ULL*[d];
    for(int i = 0; i < d; ++i){
        s1[i] = new ULL[p];
        s2[i] = new ULL[p];
    }
    ULL **s1_ps = new ULL*[d];
    for (int i = 0; i < d; ++i)
    {
        s1_ps[i] = new ULL[p];
    }

    cilk_for(int i=0;i<p;i++){
        for(int j=0;j<=d-1;j++){
            s1[j][i]=0;
        }
        js[i] = (i) * ceil((n/p));
        je[i] = (i+1<p) ? (i+1) * (ceil(n/p)) - 1:n-1;

        for(int j=js[i]; j<=je[i]; j++){
            s1 [f[j]][i]= s1 [f[j]][i] + 1;
        }
    }
    ULL* send = new ULL[p];

    for (int j =0; j<= d-1; j++){
        
        for (int i = 0; i < p; ++i)
        {
            send[i] = s1[j][i];
        }

        ULL* ret = prefix_sum(send,p);

        for (int i = 0; i < p; ++i)
        {
            s1_ps[j][i] = ret[i];
        }
    }


    cilk_for(int i=0;i<p;i++){

        ofs[i] = 1;
        for(int j=0; j<=d-1; j++){
            s2[j][i] = (i == 0) ? ofs[i] : (ofs [i] + s1_ps[j][i-1]);
            ofs[i] = ofs[i] + s1_ps[j][p-1]; 
        }

        for(int j =js[i]; j<=je[i]; j++){
            s[j] = s2[f[j]][i];
            s2[f[j]][i] = s2[f[j]][i]+1;
        }
    }
}

ULL extract_bit_segment(ULL a, ULL k, ULL l){
    return (((1 << l) - 1) & (a >> k));         
}

void par_radix_sort_with_counting_rank(ULL a[], int n, int b){

    ULL *f = new ULL[n];
    ULL *s = new ULL[n];
    ULL *B = new ULL[n];
    ULL p = 68;
    ULL q;
    ULL r = ceil(log2(ceil(n/ (p * log2(n)))));
    for(ULL k =0; k<b ; k=k+r){
        q = (k + r <= b) ? r : b-k;
        cilk_for (int i =0; i<n;i++){
           f[i] = extract_bit_segment(a[i],k,q);
        }
        par_counting_rank(f,n,q,s);

        cilk_for (int i =0;i<n;i++){
            B[s[i]-1] = a[i];
        }

        cilk_for (int i=0;i<n;i++){
            a[i] = B[i];
        }
    }       
}

bool getRandomBool(){

    int randomval = rand() % 2; 
    return randomval == 0;
} 

void Prefix_Sum(ULL* x , ULL* s, int n){

    if(n==1)
    {
        s[0] = x[0];
    }
    else
    {
        ULL z[n/2]; ULL y[n/2];

        cilk_for(int i=0;i<n/2;i++){
            y[i]=x[2*i]+x[2*i+1];           
        }

        Prefix_Sum(y, z, n/2);
        s[0]=x[0];

        cilk_for(int i=1;i<n;i++){
            if(i%2 != 0){
                s[i]=z[i/2];
            }else{
                s[i]=z[(i-1)/2] + x[i];
            }
        }
    }
}

void Prefix_Sum_serial(ULL* F, ULL* S, int n){
    
    S[0] = F[0];
    for (int i = 1; i < n; ++i)
    {
        S[i] = F[i] + S[i-1];
    }
}

void RadixSort(ULL* A, int n, int bits){

    ULL F0[n];
    ULL F1[n];
    ULL S0[n];
    ULL S1[n];
    ULL B[n];

    for (int k = 0; k < bits; ++k)
    {
        cilk_for (int i = 0; i < n; ++i)
        {
            F1[i] = SHIFT_RIGHT(A[i], k) & 1;
            F0[i] = 1 - F1[i];
        }

        Prefix_Sum(F1, S1, n);
        Prefix_Sum(F0, S0, n);

        cilk_for (int i = 0; i < n; ++i)
        {
            if (F1[i] == 0)
            {
                B[S0[i]-1] = A[i];
            }
            else
            {
                B[S0[n-1] + S1[i]-1] = A[i];
            }
        }

        cilk_for (int i = 0; i < n; ++i)
        {
            A[i] = B[i];
        }
    }
}

struct Edge
{
    int src, dest;
    double weight;
};

struct Graph
{
    int V, E;
    std::vector<Edge> edge;
};

int myComp(Edge a, Edge b)
{
    return a.weight < b.weight;
}


void Par_Simulate_Priority_CW_using_Binary_Search(int n, Edge* edgeList, int E, int* R){
    int B[n];
    int l[n]; 
    int h[n]; 
    int lo[n];
    int hi[n]; 
    int md[n];

    cilk_for (int u = 0; u < n; ++u)
    {
        l[u] = 0;
        h[u] = E-1;
    }

    for (int k = 0; k < 1+log2(E); ++k)
    {
        cilk_for (int u = 0; u < n; ++u)
        {
            B[u] = 0;
            lo[u] = l[u];
            hi[u] = h[u];
        }

        cilk_for (int i = 0; i < E; ++i)
        {
            int u = edgeList[i].src;
            if (u == n+1)
            {
                continue;
            }
            md[u] = floor((lo[u]+hi[u])/2);
            if (i >= lo[u] && i <= md[u])
            {
                B[u] = 1;
            }
        }

        cilk_for (int i = 0; i < E; ++i)
        {
            int u = edgeList[i].src;
            if (u == n+1)
            {
                continue;
            }
            md[u] = floor((lo[u]+hi[u])/2);
            if (B[u] == 1 && i >= lo[u] && i <= md[u])
            {
                h[u] = md[u];
            }
            else if (B[u] == 0 && i > md[u] && i <= hi[u])
            {
                l[u] = md[u]+1;
            }
        }

    }

    cilk_for (int i = 0; i < E; ++i)
    {
        int u = edgeList[i].src;
        if (u == n+1)
        {
            continue;
        }
        if (i == l[u])
        {
            R[u] = i;
        }
    }
}



void Par_Simulate_Priority_CW_using_Radix_Sort(int n, Edge* edgeList, int E, int* R){
    ULL* A = new ULL[E];
    ULL* A2 = new ULL[E];

    int k = ceil(log2(E)) + 1;

    cilk_for (int i = 0; i < E; ++i)
    {
        A[i] = (SHIFT_LEFT(edgeList[i].src,k)) + i;
        A2[i] = (SHIFT_LEFT(edgeList[i].src,k)) + i;
    }
    par_radix_sort_with_counting_rank(A, E, k+ceil(log2(n)));

    cilk_for (int i = 0; i < E; ++i)
    {
        int u = SHIFT_RIGHT(A[i], k);
        int j = A[i] - (SHIFT_LEFT(u, k));
        if (u < n && (i == 0 || u != (SHIFT_RIGHT(A[i-1], k))))
        {
            R[u] = j;
        }
    }
}

void addEdge(Graph* graph, int src, int dest, double weight){

	Edge e;
	e.src = src;
	e.dest = dest;
	e.weight = weight;

	Edge e2;
	e2.src = dest;
	e2.dest = src;
	e2.weight = weight;

	graph->edge.push_back(e);
	graph->edge.push_back(e2);

	graph->E = graph->edge.size();

}

struct Graph* createGraph(int V, int E)
{
    struct Graph* graph = new Graph;
    graph->V = V; 
    return graph;
}

std::mutex m;

int loopcount = 0;

void MST(int n, Graph* graph){
    
    __cilkrts_set_param("nworkers","256");
    int workers = __cilkrts_get_nworkers();
    cout<<"Setting the number of processors to:  "<<workers<<endl;
    int* L = new int[n];
    int* C = new int[n];
    int* R = new int[n];
    int* R2 = new int[n];
    int E = graph->E;
    bool* MST = new bool[E];

    Edge* edgeList = new Edge[graph->E];
    Edge* edgeList_bk = new Edge[graph->E];
    std::vector<Edge> answer;

    cilk_for (int i = 0; i < graph->E; ++i)
    { 
        edgeList_bk[i].src = graph->edge[i].src;
        edgeList_bk[i].dest = graph->edge[i].dest;
        edgeList_bk[i].weight = graph->edge[i].weight;    
    }
    std::sort(edgeList_bk, edgeList_bk+E, &myComp);    



    auto start = std::chrono::high_resolution_clock::now();    

    cilk_for (int i = 0; i < graph->E; ++i)
    {
        edgeList[i].src = graph->edge[i].src;
        edgeList[i].dest = graph->edge[i].dest;
        edgeList[i].weight = graph->edge[i].weight;    

        edgeList_bk[i].src = graph->edge[i].src;
        edgeList_bk[i].dest = graph->edge[i].dest;
        edgeList_bk[i].weight = graph->edge[i].weight;  
        
    }

    cilk_for(int i = 0; i < E; ++i){
        MST[i] = false;
    }

    std::sort(edgeList, edgeList+E, &myComp);

    cilk_for (int v = 0; v < n; ++v)
    {
        L[v] = v;
    }
    bool F = (graph->E > 0) ? true : false;

    while(F){
        cilk_for (int v = 0; v < n; ++v)
        {
            C[v] = getRandomBool();
        }
      	Par_Simulate_Priority_CW_using_Binary_Search(n, edgeList, E, R);
	//Par_Simulate_Priority_CW_using_Radix_Sort(n, edgeList, E, R)
        cilk_for (int i = 0; i < graph->E; ++i)
        {
            int u = edgeList[i].src;
            int v = edgeList[i].dest;
            if (u < n && v < n && C[u] == 1 && C[v] == 0 && R[u] == i)
            {
                L[u] = v;
                MST[i] = true;
            }
        }

        cilk_for (int i = 0; i < graph->E; ++i)
        {
            int b_src = edgeList[i].src;
            int b_dest = edgeList[i].dest;

            if (b_src < n && b_dest < n && L[b_src] != L[b_dest])
            {       
                edgeList[i].src = L[b_src];
                edgeList[i].dest = L[b_dest];                
            }
            else
            {
                edgeList[i].src = n+1;
                edgeList[i].dest = n+1;                
            }

        }

        F = false;
        cilk_for (int i = 0; i < graph->E; ++i)
        {
            if (edgeList[i].src != edgeList[i].dest)
            {
                F = true;
            }
        }

    }

    auto end = std::chrono::high_resolution_clock::now();
    long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
    long long seconds = std::chrono::duration_cast<std::chrono::seconds>(end-start).count();

    double totalwt = 0;
    int countEdges = 0;
    for (int i = 0; i < E; ++i)
    {
        if (MST[i])
        {
            countEdges++;
            totalwt += edgeList[i].weight;            
        }
    }
    cout << endl;

    std::ofstream myFile( "com-lj-binary.txt" );
    myFile << countEdges << " " << std::fixed << std::setprecision(4) << totalwt << endl;
    for (int i = 0; i < E; ++i)
    {
        if (MST[i])
        {
            myFile << edgeList_bk[i].src+1 << " " << edgeList_bk[i].dest+1 << " " << edgeList_bk[i].weight << endl; 
        }
    }
    myFile.close();


    cout << "Time taken for the algorithm : " << microseconds << "us " << seconds << "s\n";
}

int main()
{
    
    srand((unsigned int)seconds);
    time(&seconds);
    int V; // Number of vertices in graph
    int E; // Number of edges in graph
    struct Graph* graph = createGraph(V, E);
 
    cin >> V >> E;
    for (int i = 0; i < E; ++i)
    {
        int u, v;
        float w;
        cin >> u >> v >> w;
        addEdge(graph, u-1, v-1, w);
    }

    MST(V,graph);
 
    return 0;
}
