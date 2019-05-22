#include<iostream>
#include<set>
#include<vector>
#include<map>
#include<cstdio>
#include<random>

using namespace std;

///tune number of vertices
#define MaxV 100000

///tune number of edges
#define MaxE 999999

///tune max weight of edges
#define MaxW 1000000

///DSU Parameter

int root[MaxV+1];
int size[MaxV+1];

/// find_root : get root of u'subtree
int find_root(int u){
    while(u != root[root[u]]){
        u = root[root[u]];
    }
    return u;
}

///unify : add edge to graph
void unify(int u,int v){

    int root_u = find_root(u);     ///root of u's subtree
    int root_v = find_root(v);     ///root of v's subtree

    if(size[root_u]>size[root_v]){

        root[root_v] = root_u;
        size[root_u] += size[root_v];

    }else{

        root[root_u] = root_v;
        size[root_v] += size[root_v];
    }

}

///init_unify : DSU parameter intialization
void init_unify(int V){

    for(int i=0;i<V;i++){

        root[i] = i;
        size[i] = 1;
    }

}
int connected_graph(int i,int V){
    ////first generate connected graph
    for(;i<V-1;){

        int u = rand()%V;
        int v = rand()%V;
        int w = 1+rand()%MaxW;

        if(find_root(u) == find_root(v)){
            ////Edge in same component
            continue;
        }
        printf("%d %d %d\n",u,v,w);
        unify(u,v);
        ++i;
    }
    return i;
}
int main(){


    srand(time(NULL));      ///randomize each time

    int V = 1+rand()%MaxV; ///Number of Vertices

    int E = 1+rand()%MaxE;///Number of Edges


    printf("G = (V -> %d , E -> %d)\n",V,E);

    init_unify(V);  ///DSU parameter intialization


    freopen("../input/general_graph.txt","w",stdout); ///write graph into file

    printf("%d %d\n",V,E);

    int i = 0;


    ///do you want to make connected graph?
    ///i = connected_graph(i,V)

    ///connect vertices randomly in graph
    for(;i<E;i++){

        int u = rand()%V;
        int v = rand()%V;
        int w = rand()%MaxW;

        printf("%d %d %d\n",u,v,w);

    }

    return 0;
}
