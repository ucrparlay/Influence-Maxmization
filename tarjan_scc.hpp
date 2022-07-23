#pragma once
#include <parlay/random.h>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <stack>

#include "get_time.hpp"
#include "graph.hpp"
#include "utilities.h"

using namespace std;

class TARJAN_SCC {
 private:
  const Graph& G;
  sequence<NodeId> sk;
  sequence<NodeId> low, dfn;
  size_t n, idx;
  int sk_head;
  void dfs(NodeId u, Hash_Edge & hash_edge);

 public:
  size_t cnt;
  sequence<NodeId> scc;
  void tarjan(Hash_Edge & hash_edge);
  void clear();
  TARJAN_SCC() = delete;
  TARJAN_SCC(const Graph& graph) : G(graph) {
    low = sequence<NodeId>(G.n);
    dfn = sequence<NodeId>(G.n);
    scc = sequence<NodeId>(G.n);
    sk = sequence<NodeId>(G.n);
    n = G.n;
    cnt = 0;
    idx = 0;
    sk_head = 0;
  }
};

void TARJAN_SCC::dfs(NodeId u, Hash_Edge& hash_edge) {
  low[u] = dfn[u] = ++idx;
  sk[sk_head++] = u;
  for (size_t i = G.offset[u]; i < G.offset[u + 1]; i++) {
    NodeId v = G.E[i];
    float w = G.W[i];
    if (!hash_edge(u,v,w)){continue;}
    if (!dfn[v]) {
      dfs(v, hash_edge);
      low[u] = min(low[u], low[v]);
    } else if (!scc[v]) {
      low[u] = min(low[u], dfn[v]);
    }
  }
  if (low[u] == dfn[u]) {
    cnt++;
    while (1) {
      if (sk.empty()){
        cout << "pop from empty stack" << endl;
      }
      NodeId x = sk[--sk_head];
      scc[x] = cnt;
      if (x == u) break;
    }
  }
}

void TARJAN_SCC::clear() {
  for (size_t i = 0; i < n; i++) {
    dfn[i] = 0;
    scc[i] = 0;
  }
  idx = 0;
  cnt = 0;
  sk_head = 0;
}

void TARJAN_SCC::tarjan(Hash_Edge & hash_edge) {
  for (size_t i = 0; i < n; i++) {
    if (!dfn[i]) dfs(i, hash_edge);
  }
}
