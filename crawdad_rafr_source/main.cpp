#include"main.hpp"
#include"error.hpp"
#include"getRealTrace.hpp"
#include"init.hpp"
#include"maniGraph.hpp"
#include"method.hpp"
#include"output.hpp"
#include"simulation.hpp"



void solve(vector<double>& availability, const vector<vector<pii> >& contact_nodes, const Graph& g, const double row, vector<vector<int> >& success_rate) {
  int n = g.n;
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
    Graph g_off(n, g.valid_id[0], g.valid_id[1]);
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

      double avail = 0;
      rep(i, n) {
        if (mt.have_data[i] && !off.have_data[i]) ++avail;
      }
      avail /= num_cache + 1; // source
      availability[method_id] += avail / num_simu;

      // declare a requesting node
      int req = rand() % n;
      while (mt.have_data[req] || off.have_data[req]) {
        req = rand() % n;
      }

      int success_ut = simulation(contact_nodes, end_ut, g_off, mt.have_data, off.have_data, req, start_ut);
      if (success_ut >= 0) {
        ++success_rate[method_id][success_ut];
      }
    } // rep(method_id, )
  } // rep(simu_id, )

  // outputDataToFile is called in main
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  cout.tie(0);
  cout << fixed << setprecision(12);

  init();
  vector<vector<pii> > contact_nodes(300000); // 時間引数

  // declare a graph
  Graph g(n, end_vld_id, start_vld_id);
  // construct the graph
  getRealTrace(contact_nodes, end_ut, g, start_ut);
  makeDFromLambda(g);
  solveShortest(g);

  rep(row, sz(x_p)) { // 出力の横軸 動かす変数
    // visualize the condition
    cerr << row << endl;
    if (sim_pat_id == Offline) num_off = row + 1;
    else if (sim_pat_id == Caching) num_cache = row + 1;
    else if (sim_pat_id == Candidate) {}
    else if (sim_pat_id == Alpha) {}
    else error("incorrect sim_pat_id (main.cpp)");

    // delaration
    vector<vector<int> > success_rate(num_methods, vector<int>(600000));
    vector<double> availability(num_methods);
    // simulate in this condition
    solve(availability, contact_nodes, g, row, success_rate);
    // obtain the line number in the output
    outputDataToFile(availability, row, success_rate);
  }
}
