#include"global.hpp"
#include"error.hpp"
#include"output.hpp"

void betweenness(const Graph& g, Method& mt) {
  rep(u, n) { // uを通る最短経路の個数
    if (mt.have_data[u]) continue;

    rep(s, n) rep(t, s) { // すべての頂点間
      if (u == s || u == t) continue;
      if (!g.num_shortest_paths[s][t]) continue;

      if (abs(g.d[s][t] - (g.d[s][u] + g.d[u][t])) < eps) {
        // cerr << u << " " << s << " " << t << " " << g.num_shortest_paths[s][u] << " " << g.num_shortest_paths[u][t] << " " << g.num_shortest_paths[s][t] << endl;
        mt.score[u].F += g.num_shortest_paths[s][u] * g.num_shortest_paths[u][t] / g.num_shortest_paths[s][t];
      }
    }
  }
  // cerr << "Betweenness" << endl;
  // rep(i, n) cerr << mt.score[i].F << " "; cerr << endl;
}

void connectivity(const Graph& g, Method& mt) {
  // cerr << "Connectivity" << endl;
  rep(i, n) {
    rep(j, n) {
      if (i == j || g.d[i][j] >= inf - eps || g.hop[i][j] >= inf - eps) continue;
      // cerr << g.d[i][j] << " " << g.hop[i][j] << endl;
      mt.score[i].F += 1.0 / g.d[i][j] / g.hop[i][j];
      // if (i == 0) cerr << "conn " << i << " " << j << " " << g.hop[i][j] << " " << mt.score[i].F << endl;
      // if (i == 0) cerr << j << " ";
    }
    // if (i == 0) cerr << endl;
  }
  // sort(all(mt.score));
  // rep(i, n) cerr << "conn " << mt.score[i].S << " " << mt.score[i].F << endl;

  // rep(i, n) cerr << mt.score[i].F << " "; cerr << endl;
}

void degree(const Graph& g, Method& mt) {
  // cerr << "Degree" << endl;
  // 隣接ノードの重みの和
  rep(i, n) {
    for (int j : g.g[i]) {
      // cerr << i << " " << j << " " << g.lambda[i][j] << endl;
      mt.score[i].F += g.lambda[i][j]; // 1.0 / g.d[i][j];
      // if (abs(g.lambda[i][j] - 1.0 / g.d[i][j]) > eps) cerr << i << " " << j << endl;
      // lambda != 1/d, 経由(hop!=1)したほうが距離小さくなったり
      // if (i == 0) cerr << "deg " << i << " " << j << " " << g.hop[i][j] << " " << mt.score[i].F << endl;
      // if (i == 0) cerr << j << " ";
    }
    // if (i == 0) cerr << endl;
  }
  // sort(all(mt.score));
  // rep(i, n) cerr << "deg " << mt.score[i].S << " " << mt.score[i].F << endl;

  // rep(i, n) cerr << mt.score[i].F << " "; cerr << endl;
}

struct UnionFind {
  vector<int> data;
  int n;
  UnionFind(int n) : data(n, -1), n(n) {}
  bool connect(int x, int y) {
      if ((x = root(x)) == (y = root(y))) return false;
      if (data[x] > data[y]) swap(x, y);
      data[x] += data[y]; data[y] = x; n--;
      return true;
  }
  bool issame(int x, int y) { return root(x) == root(y); }
  int root(int x) { return data[x] < 0 ? x : data[x] = root(data[x]); }
  int size(int x) { return -data[root(x)]; }
  int size() { return n; }
};

// キャッシングノードはオフラインノードを避けて選ばない
// Availabilityが1になる
// optimalだけは別
void opt(const Graph &g, Method &mt, Method &off) { // g_off
    // vector<double> S(n); // とりあえず他のノードとの距離の総和
    // rep(i, n) {
    //     rep(j, n) {
    //         if (g.d[i][j] > inf - eps) continue;
    //         S[i] += g.d[i][j];
    //     }
    // }

    // cerr << "opt/off "; rep(i, n) if (!g.g[i].size()) cerr << i << " "; cerr << endl;
    // cerr << "opt/g.size "; rep(i, n) cerr << g.g[i].size() << " "; cerr << endl;

    // 下でsourceを使う
    int source = -1;
    assert(accumulate(all(mt.have_data), 0) == 1);
    rep(i, n) if (mt.have_data[i]) { source = i; break; }
    assert(source != -1);

    // vector<int> covered(n);
    // UnionFind uni(n);
    // rep(i, n) {
    //     for (int j : g.g[i]) uni.connect(i, j);
    // }
    //
    // // sourceとつながってるの
    // rep(i, n) {
    //     if (uni.issame(i, source)) covered[i] = 1;
    //     else {
    //         if (!off.have_data[i]) {
    //             cerr << "components" << endl;
    //             exit(0);
    //         }
    //     }
    // }
    //
    // while (accumulate(all(covered), 0) != n - num_off) {
    //     rep(i, n) {
    //         if (off.have_data[i]) continue;
    //         if (!covered[i]) {
    //             mt.have_data[i] = 1;
    //             rep(j, n) {
    //                 if (uni.issame(i, j)) covered[j] = 1;
    //             }
    //             break;
    //         }
    //     }
    // }

    // show caching nodes
    // cerr << "opt cache ";
    // rep(i, n) {
    //     if (mt.have_data[i]) cerr << i << " ";
    // } cerr << endl;

    double mi = 100.0 * inf; // 78ノード分
    vector<int> mi_pr; // node pairs with minimal sum value
    vector<int> pr({ source });
    auto dfs = [&](auto dfs, int i, int _d) -> void {
        if (_d == num_cache) {
            // sumを計算
            vector<int> hasCache(n);
            for (auto vi : pr) hasCache[vi] = 1;

            double sum = 0;
            for (auto vi : pr) { // caching nodes, source node
                rep(j, n) {
                    if (hasCache[j]) continue;
                    // if (abs(g.d[vi][j] - inf) < eps) continue;

                    sum += g.d[vi][j];
                }
            }
            if (chmin(mi, sum)) {
                mi_pr = pr;
            }
            return;
        }
        rep3(j, i + 1, n) {
            if (off.have_data[j] || j == source) continue; 

            pr.emplace_back(j);
            dfs(dfs, j, _d + 1);
            pr.pop_back();
        }
    };
    dfs(dfs, -1, 0);

    // rep(i, n) {
    //     if (!g.g[i].size()) continue; // オフラインノードは除く
    //     rep3(j, i + 1, n) {
    //         if (!g.g[j].size()) continue;
    //         rep3(k, j + 1, n) { // キャッシングノードを3つ選ぶ, すべての組み合わせ
    //             if (!g.g[k].size()) continue;
    //             double sum = 0;
    //             rep(l, n) {
    //                 if (!g.g[l].size()) continue;
    //                 if (l == i || l == j || l == k || l == source) continue;
    //                 // double temp = min({ g.d[l][i], g.d[l][j], g.d[l][k], g.d[l][source] });
    //                 // // if (i == 0 && j == 1 && k == 2) cerr << "opt/l temp " << l << " " << g.d[l][i] << " " << g.d[l][j] << " " << g.d[l][k] << " " << g.d[l][source] << endl;
    //                 // assert(temp < inf + eps);
    //                 // sum += temp; // オフラインノードを除いてる
    //                 double temp = g.d[l][i];
    //                 if (temp < inf - eps) sum += temp; // infもだめ
    //                 temp = g.d[l][j];
    //                 if (temp < inf - eps) sum += temp;
    //                 temp = g.d[l][k];
    //                 if (temp < inf - eps) sum += temp;
    //                 temp = g.d[l][source];
    //                 if (temp < inf - eps) sum += temp;
    //             }
    //
    //             // if (i == 0 && j == i + 1 && k == j + 1) cerr << "calcOpt/sum " << sum << " " << " mi " << mi << " " << mi - sum << endl;
    //             if (chmin(mi, sum)) {
    //                 mi_pr = vector<int>({i, j, k});
    //             }
    //         }
    //     }
    // }
    // cerr << "mi " << mi << " mi_pr " << mi_pr << endl;

    for (auto i : mi_pr) {
        mt.have_data[i] = 1;
    }
}

// void opt(const Graph &g, Method &mt) {
//     int source = -1;
//     rep(i, n) if (mt.have_data[i]) { source = i; break; }
//     assert(source != -1);
//
//     vector<int> covered(n);
//     for (int i : g.g[source]) covered[i] = 1;
//     covered[source] = 1;
//
//     [&]() {
//         while (1) {
//             rep(i, n) {
//                 if (covered[i] == 0) break;
//                 if (i == n - 1) return; // all covered
//             }
//
//             int ma = 0, nid = -1; // nxt id
//             rep(i, n) {
//                 if (covered[i]) continue;
//                 int cnt = 1; // itself
//                 for (int j : g.g[i]) {
//                     if (!covered[j]) ++cnt;
//                 }
//                 if (chmax(ma, cnt)) {
//                     nid = i;
//                 }
//             }
//
//             // 新しいキャッシングノードが決まった
//             assert(nid != -1);
//             for (int j : g.g[nid]) covered[j] = 1;
//             covered[nid] = 1;
//         }
//     }();
//
//     // キャッシングノードが余ってた場合
//
// }

// parent[s][cur] : s始点のダイクストラでcurの親
vector<int> getPath(const Graph& g, int s, int t) {
    vector<int> path;
    for (int cur = t; cur != -1; cur = g.parent[s][cur]) {
        path.eb(cur);
    }
    reverse(all(path)); // 逆順なのでひっくり返す
    return path;
}

double calcA(const Graph& g, int k, const vector<int>& path) {
  double A = 1;
  rep(i, sz(path) - 1) {
    if (i == k) continue;
    int a = path[i], b = path[i + 1];
    A *= g.lambda[a][b] / (g.lambda[a][b] + g.lambda[path[k]][path[k + 1]]);
  }
  return A;
}

double calcP(const Graph& g, const int s, const int t) {
  double p = 0;
  vector<int> path = getPath(g, s, t); // s, ..., t
  // if (s == 0 && row == 2) cerr << s << " " << t << "    " << path << endl;
  const int T = 340808 - 173507;
  rep(k, sz(path) - 1) {
    double A = calcA(g, k, path);
    double B = exp(-g.lambda[path[k]][path[k + 1]] * T);
    p += A * (1 - B);
    // if (s == 0 && row == 2) cerr << s << " " << t << "    " << A << " " << B << " " << p << endl;
  }
  // if (s == 0 && row == 2) cerr << s << " " << t << "    " << p << endl;
  return p;
}

void rafrIte(const Graph& g, Method& mt, const int num_copies) {
  // cerr << "RAFR" << endl;
  // 中心ノードで候補を絞る
  vector<int> is_cand(n);

  int vs;
  rep(i, n) if (mt.have_data[i]) vs = i;
  // cerr << "vs " << vs << endl;

  vector<vector<double> > p(n, vector<double>(n));
  rep(s, n) rep3(t, s + 1, n) {
    p[s][t] = p[t][s] = calcP(g, s, t);
  }

  // if (row == 2) {
  //     rep(j, n) cerr << p[0][j] << " "; cerr << endl;
  // }

  { // normalize
    double ma = 0;
    rep(i, n) rep3(j, i + 1, n) chmax(ma, p[i][j]);
    rep(i, n) rep3(j, i + 1, n) p[i][j] /= ma;
  }

  rep(_, mt.cand) {
    mt.init_score();
    rep(i, n) mt.score[i].S = i;

    rep(i, n) {
      if (is_cand[i] || i == vs) continue;

      const int flg = 1;
      if (flg == 0) { // degree
        for (int j : g.g[i]) {
          // if (is_cand[j]) continue;
          mt.score[i].F += 1.0 / g.d[i][j];
        }
      } else if (flg == 1) { // weighted connectivity
        rep(j, n) {
          if (is_cand[j] || j == vs || i == j || g.d[i][j] >= inf - eps || g.hop[i][j] >= inf - eps) continue;
          mt.score[i].F += p[i][j] / g.d[i][j] / g.hop[i][j];
          // cerr << p[i][j] << endl;
          // cerr << i << " " << j << " " << score[i].F << endl;
        }
      }
    }

    if (_ == 0 && row == 2) {
        // rep(i, n) cerr << mt.score[i].F << " "; cerr << endl;
        outputMetricScore(mt);
    }
    sort(all(mt.score));
    assert(mt.score.back().F < inf);
    // cerr << "Candidate " << _ << " " << score.back().S << " " << score.back().F << endl;
    is_cand[mt.score.back().S] = 1; // is_candidate
  }
  // cerr << "cand "; rep(i, n) if (is_cand[i]) cerr << i << " "; cerr << endl;

  // キャッシュノードを候補ノードから選ぶ
  // データを持つノード間の距離を離す
  {
    rep(_, num_copies) {
      mt.init_score();
      rep(i, n) {
        if (!is_cand[i] || i == vs || mt.have_data[i]) continue;
        rep(j, n) {
          if (i == j || g.d[i][j] >= inf - eps) continue;
          if (!mt.have_data[j] && j != vs) continue;
          mt.score[i].F += g.d[i][j];
          // cerr << i << " " << j << " " << g.d[i][j] << endl;
        }
      }

      // if (_ == 1 && row == 2) {
      //     // rep(i, n) cerr << mt.score[i].F << " "; cerr << endl;
      //     outputMetricScore(mt);
      // }
      sort(all(mt.score));
      assert(mt.score.back().F < inf);
      // rep(i, n) cerr << mt.score[i].S << "-" << mt.score[i].F << " "; cerr << endl;
      mt.have_data[mt.score.back().S] = 1;
    }
  }
}

void dealIte(const Graph& g, Method& mt, const int num_copies) {
  int n = g.n;
  switch(mt.method_id) {
    case RAFR:
      rafrIte(g, mt, num_copies);
      break;
  }
}

void dealNonIte(const Graph& g, Method& mt, const int num_copies) {
  int n = g.n;
  // scoreを作る
  switch(mt.method_id) {
    case BETWEENNESS:
      betweenness(g, mt);
      break;
    case CONNECTIVITY:
      connectivity(g, mt);
      break;
    case DEGREE:
      degree(g, mt);
      break;
    default:
      assert(0);
  }

  rep(i, n) if (mt.have_data[mt.score[i].S]) mt.score[i].F = 0;
  sort(all(mt.score), greater<>());
  // rep(i, n) cerr << mt.score[i] << endl;
  assert(mt.score[0].F < inf);
  // if (mt.method_id == DEGREE) rep(i, n) cerr << "degree " << " " << mt.score[i].S << " " << mt.score[i].F << endl;
  // if (mt.method_id == CLOSENESS) rep(i, n) cerr << "closeness " << " " << mt.score[i].S << " " << mt.score[i].F << endl;

  if (!mt.is_pro) {
    rep(i, num_copies) mt.have_data[mt.score[i].S] = 1;
  } else {
    double ma = mt.score[0].F;
    rep(i, n) mt.score[i].F = mt.score[i].F / ma * mt.alpha;

    [&]() {
      int cnt = 0;
      while (1) {
        rep(i, n) {
          if (cnt == num_copies) return;
          if (mt.have_data[mt.score[i].S]) continue;
          if (mt.score[i].F >= rand() % 100 / 100.0) {
            mt.have_data[mt.score[i].S] = 1;
            ++cnt;
          }
        }
      }
    }();
  }
}

void callMethods(const Graph& g, Method& mt, const int num_copies, Method &off) {
  mt.init_score();

  // have_dataを完成させる
  if (mt.method_id == OPT) {
    opt(g, mt, off);
  } else if (!mt.is_ite) {
    dealNonIte(g, mt, num_copies);
  } else {
    dealIte(g, mt, num_copies);
  }
  // cerr << "have    "; rep(i, n) if (mt.have_data[i]) cerr << i << " "; cerr << endl;
}
