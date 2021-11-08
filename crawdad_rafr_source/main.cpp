#include"global.hpp"

#include"error.hpp"
#include"getRealTrace.hpp"
#include"init.hpp"
#include"maniGraph.hpp"
#include"output.hpp"
#include"solve.hpp"

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
    else if (sim_pat_id == Candidate) { /* 別の場所で処理 */ }
    else if (sim_pat_id == Alpha) {}
    else error("incorrect sim_pat_id (main.cpp)");

    // simulate in this condition
    solve(contact_nodes, g, row);
    // obtain the line number in the output
    outputDataToFile(availability, row, success_rate);
  }
}
