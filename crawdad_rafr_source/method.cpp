#include"main.hpp"

void betweenness(const Graph& g, Method& mt) {
  int n = g.n;
  rep(u, n) { // uを通る最短経路の個数
    if (mt.have_data[u]) continue;

    rep(s, n) rep(t, s) { // すべての頂点間
      if (u == s || u == t) continue;
      if (!g.num_shortest_paths[s][t]) continue;
      if (abs(g.d[s][t] - (g.d[s][u] + g.d[u][t])) < eps) mt.score[u].F += g.num_shortest_paths[s][u] * g.num_shortest_paths[u][t] / g.num_shortest_paths[s][t];
    }
  }
}

void connectivity(const Graph& g, Method& mt) {
  int n = g.n;
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
}

void degree(const Graph& g, Method& mt) {
  int n = g.n;
  // 隣接ノードの重みの和
  rep(i, n) {
    for (int j : g.g[i]) {
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
}

void rafrIte(const Graph& g, Method& mt, const int num_copies) {
  int n = g.n;

  // 中心ノードで候補を絞る
  vector<int> is_cand(n);
  rep(i, n) if (mt.have_data[i]) is_cand[i] = 1;

  rep(_, mt.cand) {
    vector<pdi> score(n);
    rep(i, n) score[i].S = i;
    rep(i, n) {
      if (is_cand[i]) continue;
      for (int j : g.g[i]) {
        if (is_cand[j]) continue;
        score[i].F += 1.0 / g.d[i][j];
      }

      // rep(j, n) {
      //   if (i == j || g.d[i][j] >= inf - eps || g.hop[i][j] >= inf - eps) continue;
      //   score[i].F += 1.0 / g.d[i][j] / g.hop[i][j];
      // }
    }

    sort(all(score));
    assert(score.back().F < inf);
    is_cand[score.back().S] = 1; // is_candidate
  }
  // rep(i, n) if (is_cand[i]) cerr << i << " "; cerr << endl;

  // キャッシュノードを候補ノードから選ぶ
  // データを持つノード間の距離を離す
  rep(_, num_copies) {
    mt.init_score();
    rep(i, n) {
      if (!is_cand[i] || mt.have_data[i]) continue;
      rep(j, n) {
        if (i == j || g.d[i][j] >= inf - eps || !mt.have_data[j]) continue;
        mt.score[i].F += g.d[i][j];
      }
    }

    sort(all(mt.score));
    assert(mt.score.back().F < inf);
    mt.have_data[mt.score.back().S] = 1;
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
  int n = g.n;
  mt.init_score();

  // have_dataを完成させる
  if (!mt.is_ite) {
    dealNonIte(g, mt, num_copies);
  } else {
    dealIte(g, mt, num_copies);
  }
}
