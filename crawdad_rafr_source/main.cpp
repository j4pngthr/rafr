#include"global.hpp"

#include"error.hpp"
#include"getRealTrace.hpp"
#include"init.hpp"
#include"maniGraph.hpp"
#include"output.hpp"
#include"solve.hpp"

void setNumSimPat(int row) {
  if (row < 0) error("row is minus (main.cpp)");

  if (sim_pat_id == Offline) num_off = row + 1;
  else if (sim_pat_id == Caching) num_cache = row + 1;
  else if (sim_pat_id == Candidate) { /* 別の場所で処理 */ }
  else if (sim_pat_id == Alpha) {}
  else error("incorrect sim_pat_id (main.cpp)");
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  cout.tie(0);
  cout << fixed << setprecision(12);

  init();
  vector<vector<pii> > contact_nodes(300000); // 時間引数

  // declare a graph
  // cerr << start_vld_id << " "  << end_vld_id << endl;
  Graph g(n);
  // construct the graph
  cerr << "befGetReal" << endl;
  getRealTrace(contact_nodes, g);
  cerr << "befMak" << endl;
  makeDFromLambda(g);
  cerr << "befSol" << endl;
  solveShortest(g);

  rep(row, sz(x_p)) { // 出力の横軸 動かす変数
    // visualize the condition
    cerr << "row " << row << " (main.cpp)" << endl;
    setNumSimPat(row);

    // simulate in this condition
    solve(contact_nodes, g, row);
    // obtain the line number in the output
    outputDataToFile(availability, row, success_rate);
  }
}
