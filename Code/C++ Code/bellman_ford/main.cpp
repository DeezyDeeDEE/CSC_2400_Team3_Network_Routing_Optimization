#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <limits>
#include <unordered_map>
using namespace std;
using Clock = chrono::high_resolution_clock;
struct Edge { int u, v, w; };
struct RunResult { long long edges; size_t nodes; double loadTime; double algoTime; int source; int target; int distance; vector<int> path; };
static double secs(Clock::time_point a, Clock::time_point b){ return chrono::duration<double>(b-a).count(); }

vector<Edge> loadEdges(const string& fileName, long long maxEdges, unordered_set<int>& nodes, double& loadTime){
    auto start=Clock::now(); ifstream file(fileName); if(!file) throw runtime_error("Could not open dataset. Use unzipped as-skitter.txt.");
    vector<Edge> edges; string line; long long count=0;
    while(getline(file,line)){
        if(line.empty() || line[0]=='#') continue; stringstream ss(line); int u,v;
        if(ss>>u>>v){ edges.push_back({u,v,1}); edges.push_back({v,u,1}); nodes.insert(u); nodes.insert(v); count++; }
        if(maxEdges>0 && count>=maxEdges) break;
    }
    loadTime=secs(start,Clock::now()); return edges;
}

vector<int> buildPath(int source,int target,const unordered_map<int,int>& prev){
    vector<int> path; int cur=target; path.push_back(cur);
    while(cur!=source){ auto it=prev.find(cur); if(it==prev.end()) return {}; cur=it->second; path.push_back(cur); }
    reverse(path.begin(),path.end()); return path;
}

RunResult runBellmanFord(const string& fileName,int source,long long maxEdges){
    unordered_set<int> nodes; double loadTime=0; auto edges=loadEdges(fileName,maxEdges,nodes,loadTime);
    if(!nodes.count(source)) throw runtime_error("Source node not found.");
    int target=*max_element(nodes.begin(),nodes.end());
    const int INF=numeric_limits<int>::max()/4; unordered_map<int,int> dist, prev; dist.reserve(nodes.size()); prev.reserve(nodes.size());
    for(int n:nodes) dist[n]=INF; dist[source]=0;
    auto start=Clock::now(); size_t V=nodes.size();
    for(size_t i=1;i<V;i++){
        bool changed=false;
        for(const auto& e:edges){ if(dist[e.u]!=INF && dist[e.u]+e.w<dist[e.v]){ dist[e.v]=dist[e.u]+e.w; prev[e.v]=e.u; changed=true; } }
        if(!changed) break;
    }
    double algoTime=secs(start,Clock::now());
    vector<int> path=buildPath(source,target,prev);
    int d = dist.count(target)?dist[target]:INF;
    return {(long long)edges.size()/2,nodes.size(),loadTime,algoTime,source,target,d,path};
}

void makeSvgBar(const string& filename,const string& title,const string& label,double value,const string& unit){
    ofstream out(filename); double maxv=max(1.0,value); int bar=(int)(500*(value/maxv));
    out<<"<svg xmlns='http://www.w3.org/2000/svg' width='800' height='300'>\n";
    out<<"<rect width='100%' height='100%' fill='white'/>\n<text x='30' y='40' font-size='24' font-family='Arial'>"<<title<<"</text>\n";
    out<<"<text x='30' y='100' font-size='18' font-family='Arial'>"<<label<<"</text>\n";
    out<<"<rect x='200' y='75' width='"<<bar<<"' height='50' fill='steelblue'/>\n";
    out<<"<text x='"<<210+bar<<"' y='108' font-size='16' font-family='Arial'>"<<fixed<<setprecision(4)<<value<<" "<<unit<<"</text>\n</svg>\n";
}

void saveOutputs(const RunResult& r){
    ofstream csv("results.csv"); csv<<"algorithm,edges,nodes,load_time_seconds,algorithm_time_seconds,total_time_seconds,source,target,distance,path_length\n";
    csv<<"Bellman-Ford,"<<r.edges<<","<<r.nodes<<","<<r.loadTime<<","<<r.algoTime<<","<<r.loadTime+r.algoTime<<","<<r.source<<","<<r.target<<","<<r.distance<<","<<r.path.size()<<"\n";
    makeSvgBar("runtime_graph.svg","Bellman-Ford Runtime","Algorithm time",r.algoTime,"seconds");
    makeSvgBar("path_length_graph.svg","Bellman-Ford Path Length","Path length",(double)r.path.size(),"nodes");
}

int main(int argc,char* argv[]){
    try{
        string file="as-skitter.txt"; int source=0; long long maxEdges=10000;
        if(argc>=2) file=argv[1]; if(argc>=3) source=stoi(argv[2]); if(argc>=4) maxEdges=stoll(argv[3]);
        cout<<"=============================================\nBellman-Ford Algorithm Project\nStanford SNAP as-Skitter Dataset\n=============================================\n\n";
        cout<<"Note: Bellman-Ford is very slow on huge graphs. Use a smaller edge limit like 10000 or 50000.\n\n";
        auto r=runBellmanFord(file,source,maxEdges);
        cout<<"Nodes Loaded       : "<<r.nodes<<"\nEdges Loaded       : "<<r.edges<<"\nGraph Load Time    : "<<fixed<<setprecision(4)<<r.loadTime<<" seconds\n";
        cout<<"Bellman-Ford Time  : "<<r.algoTime<<" seconds\nTotal Runtime      : "<<r.loadTime+r.algoTime<<" seconds\n";
        cout<<"Source Node        : "<<r.source<<"\nTarget Node        : "<<r.target<<"\nDistance           : "<<r.distance<<" edge(s)\nPath               : ";
        if(r.path.empty()) cout<<"unreachable"; else for(size_t i=0;i<r.path.size();i++){ cout<<r.path[i]<<(i+1<r.path.size()?" -> ":""); }
        cout<<"\n\nTime Complexity    : O(V * E)\nSpace Complexity   : O(V)\n";
        saveOutputs(r); cout<<"\nCreated results.csv, runtime_graph.svg, and path_length_graph.svg\n";
    } catch(const exception& e){ cerr<<"Error: "<<e.what()<<"\n"; return 1; }
}
