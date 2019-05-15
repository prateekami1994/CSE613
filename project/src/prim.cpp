#include<iostream>
#include<set>
#include<vector>
#include<cstdio>
#include<sys/stat.h>
#include<unistd.h>
#include<string>
#include<map>

using namespace std;

///structure of edge
#define Edge  pair<int,pair<int,int > >

///Edge information Parameter
set< Edge > discovered_edges;           /// set of discovered_edges of graph
set< Edge > ::iterator sit;             /// iterator of set

///Graph parameters
vector<map<int,int> > graph;            ///graph information
map<int,int> ::iterator it;             ///iterator of edges

///DSU Parameters
vector<int> root;                       ///root of subtree
vector<int> size;                       ///size of subtree

///DFS Paramenter
vector<bool> visited;   ///mark visited or not
map<int,int> mp;
map<int,int> rmp;
vector<int> vertices;

/// __make : initiazer for dsu parameters
void __make(int V){

    root = vector<int>(V);
    size = vector<int>(V);

    for(int i=0;i<V;i++){

        root[i] = i;        ///root of i => i(self root)
        size[i] = 1;        ///size of i => 1(self vert)
    }
}

/// __find : root finder of vertex(u)
int __find(int u){
    ///distance doubling technique
    while(u != root[root[u]]){
        u = root[root[u]];
    }
    return u;
}
/// __union : unifies vertices u and v
void __union(int u,int v){
    int root_u = __find(u); ///find root of u'subtree
    int root_v = __find(v); ///find root of v'subtree
    if (root_u == root_v)
        return ; 
    if(size[root_u]>size[root_v]){  ///u'subtree is bigger than v'subtree
        root[root_v] = root_u;
        size[root_u] += size[root_v];
    }else{                          ///v'subtree is bigger than or equal u'subtree
        root[root_u] = root_v;
        size[root_v] += size[root_v];
    }
}
///disc_new_edge : insert new edges due to newly discovered vertex
void disc_new_edge(int u){
    for(it=graph[rmp[u]].begin();it!=graph[rmp[u]].end();it++)   ///adjacent edges of rmp[u]
        discovered_edges.insert({it->second,{u,mp[it->first]}});   ///add its discovered_edges
}
///add_min_edge : add minimum weight edge
int  add_min_edge(){
    int u,v,w;
    if(discovered_edges.size()){
        do{
            sit = discovered_edges.begin();         ///min weight edge
            u = sit->second.first;                  ///one of endpoint of edge
            v = sit->second.second;                 ///one of endpoint of edge
            w = sit->first;                         ///weigh of edge
            discovered_edges.erase(sit);            ///remove this edge
        } while(__find(u)==__find(v) and discovered_edges.size()); ///edge connect two different subtree
       __union(u,v);       ///update dsu
        disc_new_edge(u);    ///new discovered_edges due to u
        disc_new_edge(v);    ///new discovered_edges due to v
    }
    return w;
}

///fexists : check whether file exist or not

inline bool fexists (const std::string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}

///comparision_info : add info for comparision over several algorithm execution time
void comparision_info(double exe_time){

    freopen("../output/time.txt","a+",stdout);
    printf("%.6f ",exe_time);

}

void dfs(int u){

    vertices.push_back(u);
    visited[u] = true;

    for(auto it = graph[u].begin();it != graph[u].end(); ++it){

        int next=it->first;
        if(visited[next]==false){
            ///recure for this vertex
            dfs(next);
        }
    }
}

int main(int argc, char ** argv){

    clock_t runtime = clock();  ///run timer to read starting time of program execution

    freopen("../input/simple_graph.txt","r",stdin); ///read input

    int V,E;    /// G = (V,E) ==> Graph = (Vertices,Edges)
    scanf("%d %d",&V,&E);

    graph = vector<map<int,int> >(V+1); ///to store graph information

    int u,v,w; ///edge parameter : endpoints(u,v) and weight

    for(int i=0;i<E;i++){

        int u,v,w;
        cin >> u >> v >> w;

        graph[u][v] = w;    ///add edge(u,v) to u
        graph[v][u] = w;    ///add edge(u,v) to v

    }

    long long ans = 0;

    /// identify  components
    /// solve for component as connected sub graph
    visited = vector<bool> (V,false);
    for(int j=0;j<V;j++){

        if(visited[j]==false){      ///identified new component

            vertices.clear();
            dfs(j);

            ///rename vertex number
            ///to make independent subgraph
            mp.clear();
            rmp.clear();
            for(int i=0;i<vertices.size();i++){
                mp[vertices[i]]=i;
                rmp[i] = vertices[i];
            }

            __make(vertices.size()); ///DSU Initialization

            discovered_edges.clear(); ///Clear old edge set

            disc_new_edge(0);///add source edges

             for(int i=1;i<=vertices.size()-1;i++){

                ans += add_min_edge(); ///get min weight edge which is connecting two component
            }
        }
    }

    printf("MST Weight : %lld\n",ans);

    runtime = clock()-runtime; ///keep track of runtime
    double exe_time = double(runtime)/CLOCKS_PER_SEC;
    printf("Runtime : %.6f\n",exe_time);

    comparision_info(exe_time); ///write information for comparision

    freopen("../output/prim.txt","w",stdout);   ///write out put
    printf("MST Weight : %lld\n",ans);

    return 0;
}

