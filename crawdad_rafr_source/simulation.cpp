#include"main.hpp"
#include"simulation.hpp"

// 一番近い, データまでの距離
void computeDistToData(vector<double>& dist_to_data, const Graph& g, const vector<int>& have_data, const vector<int>& is_off_node) {
  int n = g.n;
  rep(i, n) {
    if (is_off_node[i]) continue;
    rep(j, n) {
      if (!have_data[j] || is_off_node[j]) continue;
      chmin(dist_to_data[i], g.d[i][j]);
    }
  }
}

int simulation(const vector<vector<pii> >& contact_nodes, const int end_ut, const Graph& g, const vector<int>& have_data, const vector<int>& is_off_node, int req, const int start_ut) {
  int n = g.n;
  vector<double> dist_to_data(n, inf);
  computeDistToData(dist_to_data, g, have_data, is_off_node);

  // リクエスト元の処理
  if (is_off_node[req]) return -1;
  if (have_data[req]) return 0;
  // シミュレーション
  rep3(t, start_ut, end_ut) {
    for (pii p : contact_nodes[t]) {
      int node1 = p.F, node2 = p.S;
      rep(i, 2) {
        swap(node1, node2);
        // > inf-epsでバグる
        if (is_off_node[node1] || is_off_node[node2] || g.d[node1][node2] >= inf || req != node1) continue;
        // 移動して
        if (dist_to_data[node2] < dist_to_data[node1]) {
          req = node2;
        }
        // データ持ってるノードに着いた
        if (have_data[node2]) return t - start_ut;
      }
    }
  }
  return -1;
}
