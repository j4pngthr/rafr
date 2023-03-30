#include"global.hpp"

#include"error.hpp"

struct Edge {
  int node1, node2;
  double lambda;
};

struct Edge data[] = {
  0, 1, 0.2,
  0, 2, 0.3,
  0, 3, 0.3,
  1, 3, 0.4,
  2, 3, 0.3,
  3, 4, 0.1,
  4, 5, 0.3,
  4, 6, 0.2,
  5, 6, 0.2,
  -1, -1, -1
};

void connect(Graph& g, int node1, int node2, double lambda) {
  rep(i, 2) {
    swap(node1, node2);
    g.g[node1].emplace(node2);
    g.lambda[node1][node2] = lambda;
  }
}

void getRealTrace(Graph& g) {
  struct Edge *p;
  for (p = data; p -> node1 != -1; p++) {
    int node1 = p -> node1, node2 = p -> node2;
    double lambda = p -> lambda;
    // cerr << node1 << " " << node2 << " " << lambda << endl;
    connect(g, node1, node2, lambda);
  }
}
