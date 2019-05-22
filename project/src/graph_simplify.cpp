#include<iostream>
#include<set>
#include<vector>
#include<map>
#include<cstdio>

using namespace std;

set<pair<int,int> > omitted_edges;     ///keep track of printed edges

vector<map<int,int> > graph;        ///adjacency list
map<int,int> ::iterator it;         ///map iterator


///omit_to_file : print graph edges
int omit_to_file(int u,int v,int w,bool flag=true){

    if( omitted_edges.find(make_pair(u,v))!=omitted_edges.end() ||

        omitted_edges.find(make_pair(v,u))!=omitted_edges.end()){         ///edge is already printed

            return 0;
    }else{                                                          ///edge is not already printed

        if(flag){                                                   ///called by omit_to_file function to print edge
            printf("%d %d %d\n", u, v, w);
        }

        omitted_edges.insert(make_pair(u,v));                          ///keep track of printed edges

        return 1;
    }
}

///count_edges : to count total number of edges in graph
int count_edges(int V){

    int num_edges = 0;      ///number of edges
    omitted_edges.clear();     ///clear print once set


    for(int u=0;u<V;u++){                                   ///traverse vertices

        for(it=graph[u].begin();it!=graph[u].end();++it){   ///adjcent edges of u

            num_edges+= omit_to_file(u,it->first,it->second,false);

        }
    }
    omitted_edges.clear();     ///clear print once set

    return num_edges;

}


#include <sys/stat.h>
#include <unistd.h>
#include <string>

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
void comparision_info(int E){

    if(!fexists("../output/time.txt")){

        freopen("../output/time.txt","w",stdout);
        printf("%d ",E);
    }else{

        freopen("../output/time.txt","a+",stdout);
        printf("%d ",E);
    }
}

int main(){

    freopen("../input/general_graph.txt","r",stdin);    ///write graph into file as input

    int V,E;    /// G = (V,E)
    scanf("%d %d",&V,&E);

    graph = vector<map<int,int> >(V+1);

    int u,v,w; ///edge endpoints and weight

    for(int i=0;i<E;i++){

        scanf("%d %d %d", &u,&v,&w);

        if(u==v){ ///Self loop
            continue;
        }

        it = graph[u].find(v);

        if(it!=graph[u].end()){

            w = min(w,it->second);  ///select min weight edge

            graph[u].erase(v); ///remove old edge(u,v) from u
            graph[v].erase(u); ///remove old edge(u,v) from v
        }

        graph[u].insert(make_pair(v,w));///add edge(u,v) from u
        graph[v].insert(make_pair(u,w));///add edge(u,v) from v
    }

    E = count_edges(V);

    printf("G = (V -> %d , E -> %d)\n",V,E);


    comparision_info(E); ///write information for comparision


    freopen("../input/simple_graph.txt","w",stdout);    ///write input

    printf("%d %d\n",V, E); ///Vertices and Edges

    ///print graph to input file
    for(int i=0;i<V;i++){

        for(it = graph[i].begin();it!=graph[i].end();++it){

            omit_to_file(i,it->first,it->second);
        }
    }

    return 0;
}

