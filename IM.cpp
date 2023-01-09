#include <iostream>
// #include "IM.hpp"
#include "IM_compact.hpp"
#include "parseCommandLine.hpp"
// #include "PMC.hpp"
#include "get_time.hpp"

// ./IM /data/lwang323/graph/bin/Youtube_sym.bin -k 200 -R 200 -w 0.1 -celf
int main(int argc, char* argv[]){
    CommandLine P(argc, argv);
    if (argc < 1) {
        cerr << "Usage: " << argv[0] << " graph\n";
        abort();
    }
    char* file = argv[1];
    size_t k = P.getOptionInt("-k", 200);
    size_t R = P.getOptionInt("-R", 200);
    float w = P.getOptionDouble("-w", 0.02);
    // int option = P.getOptionInt("-option", 2);
    // bool CELF = P.getOption("-celf");
    float compact = P.getOptionDouble("-compact", 1.0);
    // thresh = P.getOptionInt("-thresh", 5);
    // Graph graph = read_txt(file);
    // graph.symmetric = true;
    Graph graph = read_graph(file);
    AssignUniWeight(graph,w);
    cout << "n: " << graph.n << " m: " << graph.m << " w: " << w << " R: " << R << " k: " << k<< endl;
    #if defined(MEM)
    cout << "**size of graph is " << sizeof(graph) + (sizeof(NodeId)+sizeof(float))*graph.m \
                              + sizeof(EdgeId)*graph.n << endl;
    #endif
    if (P.getOption("-compact")) {
      timer tt;
      t_first.reset();
      t_first.start();
      cout << "compact " << compact << endl;
      CompactInfluenceMaximizer compact_IM_solver(graph, compact, R);
      compact_IM_solver.init_sketches();
      sequence<pair<NodeId, float>> seeds;
      if (P.getOption("-Q")){
        seeds = compact_IM_solver.select_seeds_prioQ(k);
      }else if (P.getOption("-PacT")){
        seeds = compact_IM_solver.select_seeds_PacTree(k);
      }else{
        seeds = compact_IM_solver.select_seeds(k);
      }
      cout << "total time: " << tt.stop() << endl;
      cout << "seeds: ";
      for (auto t: seeds) cout << t.first << ' ';
      cout << endl;
      return 0;
    // }else if (P.getOption("-pmc")){
    //   timer t_pmc;
    //   t_pmc.start();
    //   DirectedInfluenceMaximizer pmc_solver(graph, R);
    //   pmc_solver.init_sketches_phases();
    //   // pmc_solver.init_sketches();
    //   cout << "init sketch time: " << t_pmc.stop() << endl; 
    //   timer t_select; t_select.start();
    //   auto seeds_spread = pmc_solver.select_seeds(k);
    //   cout << "select time: " << t_select.stop()  << endl;
      // for (auto x: seeds_spread) {
      //   // cout << x.first << " " << x.second << endl;
      //   cout << x.first << " ";
      // }
      // cout << endl;
    // }else{
    //   InfluenceMaximizer IM_solver(graph);
    //   timer t_init;
    //   float cost;
    //   t_init.start();
    //   IM_solver.init_sketches(R, option);
    //   cost = t_init.stop();
    //   cout << "parallel{edges} init_sketches: " << cost<< endl;
      
    //   timer t_select;
    //   auto seeds_spread = IM_solver.select_seeds(k, R, CELF);
    //   cost = t_select.stop();
    //   cout << "select time: " << cost  << endl;
    //   cout << "seeds: ";
    //   for (auto x: seeds_spread) {
    //     cout << x.first << " ";
    //   }
    //   cout << endl;
    }
    return 0;
}