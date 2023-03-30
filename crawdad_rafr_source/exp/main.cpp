#include"global.hpp"

#include"error.hpp"
#include"getRealTrace.hpp"
#include"maniGraph.hpp"
#include"solve.hpp"

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  cout.tie(0);
  cout << fixed << setprecision(12);

  // declare a graph
  // cerr << start_vld_id << " "  << end_vld_id << endl;
  Graph g(n);
  // construct the graph
  // cerr << "befGetReal" << endl;
  getRealTrace(g);
  // cerr << "befMak" << endl;
  makeDFromLambda(g);
  // cerr << "befSol" << endl;
  dijkstraAllS(g);

  // simulate in this condition
  solve(g);
}
