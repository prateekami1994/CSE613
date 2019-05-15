#include<iostream>
#include<set>
#include<vector>
#include<map>
#include<queue>
#include<cstdio>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <sstream>
#include <string>
#include <bits/stdc++.h>
using namespace std;

#define INF 1e9

std::ifstream infile("../output/time.txt");

//info : storing info about execution time
typedef struct _ {
    double E,a,b,c,d;
} info;

//__cmp : compare based on number of edges
bool __cmp(info a,info b){
    return a.E < b.E;
}

//update : set min and max range
void update(double &minA,double &maxA,double &value){

    minA = min(minA,value);     //set min value
    maxA = max(maxA,value);     //set max value

}

//main : driver function
int main(){

    string line;

    vector<info> output;        //store initial and final output

    while (std::getline(infile, line))
    {
        std::istringstream iss(line);
        double E,a,b,c,d;
        if (!(iss >> E >> a >> b >> c >> d)) { break; } // error
        //cout<<E<<" "<<a<<" "<<b<<" "<<c<<endl;
        output.push_back({E,a,b,c,d});
        // process pair (a,b)
    }

    sort(output.begin(),output.end(),__cmp);        //sort execution time data based on number of edges

    freopen("../output/time.txt","w",stdout);

    double minE=INF,maxE =-INF;                               //number of edges
    double minT=INF,maxT = -INF;                               //exe. time range

    for(int i=0;i<output.size();i++){

        update(minE,maxE,output[i].E);

        update(minT,maxT,output[i].a);

        update(minT,maxT,output[i].b);

        update(minT,maxT,output[i].c);

        update(minT,maxT,output[i].d);
    }

    printf("%f %f\n",minE,maxE);
    printf("%f %f\n",minT,maxT);

    for(int i=0;i<output.size();i++){

        printf("%f %f %f %f %f\n",output[i].E, output[i].a, output[i].b, output[i].c,output[i].d);
    }
    return 0;
}

