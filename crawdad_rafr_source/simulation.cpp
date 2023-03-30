#include"global.hpp"

#include"simulation.hpp"

// 一番近い, データまでの距離
void computeDistToData(vector<double>& dist_to_data, const Graph& g, const vector<int>& have_data, const vector<int>& is_off_node) {
  rep(i, n) {
    if (is_off_node[i]) continue;
    rep(j, n) {
      if (!have_data[j] || is_off_node[j]) continue;
      chmin(dist_to_data[i], g.d[i][j]);
    }
  }
}

// void epidemic() {
//     vector<int> reached(n);
//
//     rep3(t, start_ut, end_ut) { // global variance
//         for (pii p : contact_nodes[t]) {
//             int node1 = p.F, node2 = p.S;
//
//         }
//     }
// }

int simulation(const vector<vector<pii> >& contact_nodes, const Graph& g, const vector<int>& have_data, const vector<int>& is_off_node, int req, int method_id) {
    vector<double> dist_to_data(n, inf);
    computeDistToData(dist_to_data, g, have_data, is_off_node);
    // cerr << "aftComDist (simulation.cpp)" << endl;

    // リクエスト元の処理
    // if (is_off_node[req]) return -1;
    assert(!is_off_node[req]);

    // if (have_data[req]) return 0;
    assert(!have_data[req]);

    // cerr << "aftReqDeal (simulation.cpp)" << endl;

    // シミュレーション
    // cerr << start_ut << " " << end_ut << endl;
    vector<int> reached(n);
    reached[req] = 1;

    rep3(t, start_ut, end_ut) {
        for (pii p : contact_nodes[t]) {
            int node1 = p.F, node2 = p.S;

            // node1 to node2
            rep(i, 2) {
                swap(node1, node2);

                if (vi[method_id] == OPT) {
                    if (is_off_node[node1] || is_off_node[node2]) continue;

                    if (!reached[node1]) continue;
                    reached[node2] = 1;

                    if (have_data[node2]) return t - start_ut;
                } else {
                    // > inf-epsでバグる
                    // if (is_off_node[node1] || is_off_node[node2] || g.d[node1][node2] >= inf || req != node1) continue;
                    if (is_off_node[node1] || is_off_node[node2]) continue;
                    if (req != node1) continue;

                    // 移動して
                    if (dist_to_data[node2] < dist_to_data[node1]) {
                      req = node2;
                    }

                    //　キャッシングノードに着いた
                    if (have_data[req]) return t - start_ut;
                }

                // if (is_off_node[node1] || is_off_node[node2]) continue;
                //
                // if (!reached[node1]) continue;
                // reached[node2] = 1;
                //
                // if (have_data[node2]) return t - start_ut;
            }
        }
    }
    return -1;
}
