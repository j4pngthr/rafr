#include"global.hpp"

#include"maniGraph.hpp"
#include"method.hpp"
#include"simulation.hpp"

void calcAva(const int method_id, Method mt, Method off) {
  double avail = 0;
  rep(i, n) {
    if (mt.have_data[i] && !off.have_data[i]) ++avail;
  }
  avail /= num_cache + 1; // source
  availability[method_id] += avail / num_simu;
}

void solve(const vector<vector<pii> >& contact_nodes, const Graph& g, const double row) {
  rep(simu_id, num_simu) {
    if (simu_id % 100 == 0) cerr << "row " << row << " simu_id " << simu_id << endl;

    // delclare the offline node
    Method off(0, 1, DEGREE, n);
    off.alpha = 0.5;
    if (sim_pat_id == Alpha) off.alpha = x_p[row];

    // make offline nodes
    callMethods(g, off, num_off);
    // cerr << "off "; outputIndex(off.have_data);

    // declare a graph considering offline nodes
    Graph g_off(n);
    // construct the graph
    makeOff(g, g_off, off.have_data);

    rep(method_id, num_methods) {
      // declare the method obtaining caching nodes
      Method mt((vi[method_id] == RAFR ? 1 : 0), 0, vi[method_id], n);
      mt.cand = n / 2;
      if (sim_pat_id == Candidate) mt.cand = (int)(0.01 * x_p[row] * n);

      int source = rand() % n;
      while (off.have_data[source]) {
        source = rand() % n;
      }
      mt.have_data[source] = 1;
      // cerr << "source " << source << endl;

      // obtain caching nodes based on the graph without offline nodes
      // source is not included in caching
      callMethods(g, mt, num_cache);
      // cerr << vs[method_id] << " "; outputIndex(mt.have_data);

      calcAva(method_id, mt, off);

      // declare a requesting node
      int req = rand() % n;
      while (mt.have_data[req] || off.have_data[req]) {
        req = rand() % n;
      }

      int success_ut = simulation(contact_nodes, g_off, mt.have_data, off.have_data, req);
      if (success_ut >= 0) {
        ++success_rate[method_id][success_ut];
      }
    } // rep(method_id, )
  } // rep(simu_id, )

  // outputDataToFile is called in main
}
