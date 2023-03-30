#include"global.hpp"
#include"error.hpp"

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
  cerr << "Betweenness" << endl;
  rep(i, n) cerr << mt.score[i].F << " "; cerr << endl;
}

void connectivity(const Graph& g, Method& mt) {
  cerr << "Connectivity" << endl;
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

  rep(i, n) cerr << mt.score[i].F << " "; cerr << endl;
}

void degree(const Graph& g, Method& mt) {
  cerr << "Degree" << endl;
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

  rep(i, n) cerr << mt.score[i].F << " "; cerr << endl;
}

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
  const int T = 340808 - 173507;
  rep(k, sz(path) - 1) {
    p += calcA(g, k, path) * (1 - exp(-g.lambda[path[k]][path[k + 1]] * T));
  }
  return p;
}

void dfs(int n, int m, vector<int> v, vector<vector<int> >& vs) {
  if (sz(v) == m) {
    vs.eb(v);
    return;
  }

  int s = 0;
  if (sz(v)) s = v.back() + 1;
  rep3(i, s, n) {
    v.eb(i);
    dfs(n, m, v, vs);
    v.pop_back();
  }
}

void rafrIte(const Graph& g, Method& mt, const int num_copies) {
  // cerr << "RAFR" << endl;
  // 中心ノードで候補を絞る
  vector<int> is_cand(n);

  int vs;
  rep(i, n) if (mt.have_data[i]) vs = i;
  cerr << "vs " << vs << endl;

  vector<vector<double> > p(n, vector<double>(n));
  rep(s, n) rep3(t, s + 1, n) {
    if (s == t) continue;
    p[s][t] = calcP(g, s, t);
    p[t][s] = p[s][t];
  }
  { // normalize
    double ma = 0;
    rep(i, n) rep3(j, i + 1, n) chmax(ma, p[i][j]);
    rep(i, n) rep3(j, i + 1, n) p[i][j] /= ma;
  }

  rep(_, mt.cand) {
    // cerr << "_ " << _ << endl;
    vector<pdi> score(n);
    rep(i, n) score[i].S = i;
    rep(i, n) {
      if (is_cand[i] || i == vs) continue;

      const int flg = 1;
      if (flg == 0) { // degree
        for (int j : g.g[i]) {
          // if (is_cand[j]) continue;
          score[i].F += 1.0 / g.d[i][j];
        }
      } else if (flg == 1) { // weighted connectivity
        rep(j, n) {
          if (is_cand[j] || j == vs || i == j || g.d[i][j] >= inf - eps || g.hop[i][j] >= inf - eps) continue;
          score[i].F += p[i][j] / g.d[i][j] / g.hop[i][j];
          // cerr << p[i][j] << endl;
          // cerr << i << " " << j << " " << score[i].F << endl;
        }
      }
    }
    sort(all(score));
    assert(score.back().F < inf);
    cerr << "Candidate " << _ << " " << score.back().S << " " << score.back().F << endl;
    is_cand[score.back().S] = 1; // is_candidate
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

      sort(all(mt.score));
      assert(mt.score.back().F < inf);
      rep(i, n) cerr << mt.score[i].S << "-" << mt.score[i].F << " "; cerr << endl;
      mt.have_data[mt.score.back().S] = 1;
    }
  }

  // {
  //   vector<int> cand;
  //   rep(i, n) if (is_cand[i]) cand.eb(i);
  //
  //   vector<int> v_cache;
  //   double s_ma = 0;
    // do {
    //   vector<int> v_d;
    //   rep(i, n) if (mt.have_data[i]) v_d.eb(i);
    //   rep(i, num_copies) v_d.eb(cand[i]);
    //   sort(all(v_d));
    //   cerr << cand << "    " << v_d << endl;
    //
    //   double s = 0;
    //   rep(i, sz(v_d)) {
    //     rep3(j, i + 1, sz(v_d)) {
    //       s += g.d[v_d[i]][v_d[j]];
    //     }
    //   }
    //   if (chmax(s_ma, s)) v_cache = v_d;
    //   cerr << s << "    " << v_cache << endl;
    // } while (next_permutation(all(cand))); // 無駄だけどO(1)だからいいや


    // rep(i, sz(v_cache)) mt.have_data[v_cache[i]] = 1;
    // cerr << "v_cache s_ma " << v_cache << " " << s_ma << endl;
  // }

  // {
  //   vector<vector<int> > vs;
  //   dfs(mt.cand, num_copies, vector<int>(), vs);
  //   // cerr << n << " " << num_copies << endl;
  //   // rep(i, sz(vs)) { for (auto vsi: vs[i]) cerr << vsi << " "; cerr << endl; }
  //
  //   vector<int> v_cache;
  //   double s_ma = 0;
  //   if (num_copies > mt.cand) error("num_copies is greater than mt.cand");
  //
  //   vector<int> cand;
  //   rep(i, n) if (is_cand[i]) cand.eb(i);
  //   cerr << "cand " << cand << endl;
  //
  //   for (auto vsi : vs) {
  //     double s = 0;
  //     rep(j, sz(vsi)) {
  //       rep3(k, j + 1, sz(vsi)) {
  //         int _j = vsi[j], _k = vsi[k];
  //         s += g.d[cand[_j]][cand[_k]];
  //       }
  //     }
  //     if (chmax(s_ma, s)) v_cache = vsi;
  //   }
  //   rep(i, sz(v_cache)) v_cache[i] = cand[v_cache[i]];
  //   cerr << "v_cache " << v_cache << endl;
  //
  //   rep(i, sz(v_cache)) mt.have_data[v_cache[i]] = 1;
  // }
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

void callMethods(const Graph& g, Method& mt, const int num_copies) {
  mt.init_score();

  // have_dataを完成させる
  if (!mt.is_ite) {
    dealNonIte(g, mt, num_copies);
  } else {
    dealIte(g, mt, num_copies);
  }
  // cerr << "have    "; rep(i, n) if (mt.have_data[i]) cerr << i << " "; cerr << endl;
}
