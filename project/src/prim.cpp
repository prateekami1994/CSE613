#include<iostream>
#include<set>
#include<vector>
#include<cstdio>
#include<sys/stat.h>
#include<unistd.h>
#include<string>
#include<map>

using namespace std;

#define Edge  pair<int,pair<int,int > >

set< Edge > colored_edges;           // set of discovered_edges of graph
set< Edge > ::iterator wedge;             // iterator of set

vector<map<int,int> > graph;            //graph 
map<int,int> ::iterator it;             //iterator over edges

vector<int> root;                       ///root of subtree
vector<int> size;                       ///size of subtree

// Data structures for Depth first search
vector<bool> visited;   ///mark visited or not
map<int,int> mp;
map<int,int> rmp;
vector<int> vertices;

/// initialzer function for setting the root and the size before unification
void init_unification(int V){

    root = vector<int>(V);
    size = vector<int>(V);

    for(int i=0;i<V;i++){

        root[i] = i;        ///root of i => i(self root)
        size[i] = 1;        ///size of i => 1(self vert)
    }
}

// find the root of a given vertex
int find_root(int u){
    ///distance doubling technique
    while(u != root[root[u]]){
        u = root[root[u]];
    }
    return u;
}


// Unification of two vertexes. Uses the root and size calculated during init_unification()
void unify(int u,int v){
    int root_u = find_root(u); 
    int root_v = find_root(v); 
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

// insert new edges into the the colored_edges vector
void disc_new_edge(int u){
    for(it=graph[rmp[u]].begin();it!=graph[rmp[u]].end();it++)   ///adjacent edges of rmp[u]
        colored_edges.insert({it->second,{u,mp[it->first]}});   
}

///add_min_edge : add minimum weight edge
int  add_min_edge(){
    int u,v,w;
    if(colored_edges.size()){
        do{
            wedge = colored_edges.begin();         ///min weight edge
            u = wedge->second.first;                  ///one of endpoint of edge
            v = wedge->second.second;                 ///one of endpoint of edge
            w = wedge->first;                         ///weigh of edge
            colored_edges.erase(wedge);            ///remove this edge
        } while(find_root(u)==find_root(v) and colored_edges.size()); ///edge connect two different subtree
        unify(u,v);       ///unify
        disc_new_edge(u);    ///new discovered_edges due to u
        disc_new_edge(v);    ///new discovered_edges due to v
    }
    return w;
}


///add info for comparision over several algorithm execution time
void comparision_info(double exe_time){

    freopen("../output/time.txt","a+",stdout);
    printf("%.5f ",exe_time);

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
                mp[vertices[i]]=i; // store the vertex number using index as the vertex content
                rmp[i] = vertices[i]; // store the vertex into its own index
            }

            init_unification(vertices.size()); ///DSU Initialization

            colored_edges.clear(); ///Clear old edge set

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

