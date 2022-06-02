#include <iostream>
#include "IM.hpp"
#include "parseCommandLine.hpp"
#include "get_time.hpp"

// ./IM /data/lwang323/graph/bin/Youtube_sym.bin -k 200 -R 200 -w 0.1
int main(int argc, char* argv[]){
    CommandLine P(argc, argv);
    if (argc < 1) {
        cerr << "Usage: " << argv[0] << " graph\n";
        abort();
    }
    char* file = argv[1];
    // size_t k = P.getOptionInt("-k", 200);
    size_t R = P.getOptionInt("-R", 200);
    float w = P.getOptionDouble("-w", 0.0);
    int option = P.getOptionInt("-option", 2);
    // bool CELF = P.getOption("-celf");
    Graph graph = read_graph(file);
    if (w == 0.0){
        cout << "WIC" << endl;
        AssignIndegreeWeight(graph);
    }else{
        cout << "UIC w: " << w<<  endl;
        AssignUniWeight(graph, w);
    }
    cout << "n: " << graph.n << " m: " << graph.m << endl;
    InfluenceMaximizer IM_solver(graph);
    timer t_init;
    float cost;
    // t.start();
    // IM_solver.init_sketches(R, 0);
    // cost = t.stop();
    // cout << "parallel{parallel} init_sketches: " << cost << endl;
    // t.start();
    // IM_solver.init_sketches(R, 1);
    // cost = t.stop();
    // cout << "sequential{parallel} init_sketches: " << cost << endl;
    if (P.getOption("-w")){
        t_init.start();
        IM_solver.init_sketches(R, option);
        // IM_solver.init_sketches2(R);
        cost = t_init.stop();
        cout << "parallel{edges} init_sketches: " << cost<< endl;
    }else{
        t_init.start();
        IM_solver.init_sketches(R, option);
        // IM_solver.init_sketches2(R);
        cost = t_init.stop();
        cout << "parallel{edges} init_sketches: " << cost<< endl;
    }
    

    // timer t_select;
    // auto seeds_spread = IM_solver.select_seeds(k, R, CELF);
    // cost = t_select.stop();
    // cout << "select time: " << cost  << endl;
    // for (auto x: seeds_spread) {
    //   cout << x.first << " " << x.second << endl;
    // }    
    return 0;
}