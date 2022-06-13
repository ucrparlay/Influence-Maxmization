#include "tarjan_scc.hpp"

#include <algorithm>  // std::max
#include <unordered_set>

#include "get_time.hpp"
#include "parseCommandLine.hpp"

using namespace std;

struct hashFunction{
  size_t operator()(const pair<NodeId, NodeId>& edge) const{
    return _hash(_hash(edge.first)+edge.second);
  }
};

void SCC_status(sequence<NodeId> label, size_t n) {
  sequence<NodeId> flag(n + 1);
  parallel_for(0, n + 1, [&](NodeId i) { flag[i] = 0; });
  for (size_t i = 0; i < n; i++) {
    flag[label[i]]++;
  }

  cout << "n_scc = " << parlay::count_if(flag, [&](NodeId label_size){return label_size!=0;}) << endl;
  cout << "Largest StronglyConnectedComponents has " << parlay::reduce(flag, maxm<NodeId>()) << endl;
  
  // cout << parlay::count_if(flag, [&](NodeId label_size) {
  //   return label_size != 0;
  // }) << '\n';
  // cout << parlay::reduce(flag, maxm<NodeId>()) << '\n';
}

int main(int argc, char** argv) {
  CommandLine P(argc, argv);
  if (argc < 2) {
    cout << "usage: ./tarjan_scc fileName" << endl;
    return 0;
  }
  char* fileName = argv[1];
  Graph graph = read_graph(fileName);

  float w = P.getOptionDouble("-w", 0.0);
  if (w == 0.0){
    cout << "WIC" << endl;
    AssignIndegreeWeight(graph);
  }else{  
    cout << "UIC" << endl;
    AssignUniWeight(graph,w);
  }

  int repeat = P.getOptionInt("-t", (int)10);
  NodeId graph_id = P.getOptionInt("-i", 0);
  timer t;
  TARJAN_SCC SCC_P(graph);
  Hash_Edge hash_edge{graph_id, (NodeId)graph.n, true};
  SCC_P.tarjan(hash_edge);
  double scc_cost;
  for (int i = 0; i <= repeat; i++) {
    SCC_P.clear();
    t.start();
    SCC_P.tarjan(hash_edge);
    scc_cost = t.stop();
    if (i) {
      cout << scc_cost << endl;
    }
  }
  cout << "average cost " << t.get_total()/repeat << endl;
  if (P.getOption("-status")){
    SCC_status(SCC_P.scc, graph.n);
    unordered_set<pair<NodeId,NodeId>, hashFunction> edge_set;
    for (size_t i = 0; i<graph.n; i++){
      NodeId u = i;
      for (size_t j = graph.offset[i]; j<graph.offset[i+1]; j++){
        NodeId v = graph.E[j];
        float w = graph.W[j];
        if (hash_edge(u,v,w) && SCC_P.scc[u]!= SCC_P.scc[v]){
          edge_set.insert(make_pair(SCC_P.scc[u], SCC_P.scc[v]));
        }
      }
    }
    cout << "number of inter_scc edges = " << edge_set.size() << endl;
  }

  return 0;
}