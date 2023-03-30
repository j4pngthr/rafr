#include"global.hpp"

#include"maniGraph.hpp"
#include"method.hpp"

void outputIndex(const vector<int>& v) {
  rep(i, sz(v)) if (v[i]) cerr << i << " ";
  cerr << endl;
}

void solve(const Graph& g) {
  // delclare the offline node
  Method off(0, 1, DEGREE, n);
  off.alpha = 0.5;

  // make offline nodes
  off.have_data[5] = 1;
  // cerr << "off    "; outputIndex(off.have_data);

  rep(method_id, num_methods) {
    Method mt((vi[method_id] == RAFR ? 1 : 0), 0, vi[method_id], n);
    mt.cand = n / 2;

    int source = 5;
    // while (off.have_data[source]) {
    //   source = rand() % n;
    // }
    mt.have_data[source] = 1;
    // cerr << "source " << source << endl;

    // obtain caching nodes based on the graph without offline nodes
    // source is not included in caching
    callMethods(g, mt, num_cache);
    // cerr << vs[method_id] << "    "; outputIndex(mt.have_data);
  }
}
