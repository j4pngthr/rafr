#include"global.hpp"

#include"maniGraph.hpp"

void makeDFromLambda(Graph& g) {
    rep(i, n) rep(j, i) {
        if (g.lambda[i][j] > eps) g.d[i][j] = g.d[j][i] = 1.0 / g.lambda[i][j];
    }
}

void makeOff(const Graph& g, Graph& g_off, const vector<int>& is_off_node) {
    g_off.g = g.g;
    // dは作り直さないとダメ
    g_off.lambda = g.lambda;
    rep(i, n) { // オフラインノードをグラフに反映
        if (is_off_node[i]) {
            // cerr << "is_off_node " << i << endl;
            g_off.g[i].clear();
            rep(j, n) {
                g_off.lambda[i][j] = g_off.lambda[j][i] = 0;
                g_off.d[i][j] = g_off.d[j][i] = inf;
                g_off.g[j].erase(i); // なくても大丈夫?
            }
        }
    }
    makeDFromLambda(g_off);

    dijkstraAllS(g_off);
    // cerr << "g_off/g.size "; rep(i, n) cerr << g_off.g[i].size() << " "; cerr << endl;
    // rep(i, n) {
    //     if (i != 0 && i != 45) continue;
    //     cerr << "g_off/d " << i << "    "; rep(j, n) cerr << g_off.d[i][j] << " "; cerr << endl;
    // }
}

void dijkstra(Graph& g, const int s) {
    priority_queue<pdi, vector<pdi>, greater<pdi> > q; // 距離, 頂点
    for (int nxt : g.g[s]) { // 下でelse if通る
        // if (s == 45) cerr << "dijkstra/nxt " << nxt << endl;
        q.emplace(g.d[s][nxt], nxt);
        g.num_shortest_paths[s][nxt] = g.hop[s][nxt] = 1; // 最短経路の個数
        g.parent[s][nxt] = s;
    }

    while (!q.empty()) {
        pdi p = q.top();
        int now = p.S;
        q.pop();
        for (int nxt : g.g[now]) {
            if (chmin(g.d[s][nxt], g.d[s][now] + g.d[now][nxt])) {
                q.emplace(g.d[s][nxt], nxt);
                g.num_shortest_paths[s][nxt] = g.num_shortest_paths[s][now];
                g.hop[s][nxt] = g.hop[s][now] + 1;
                g.parent[s][nxt] = now;
            } else if (abs(g.d[s][nxt] - (g.d[s][now] + g.d[now][nxt])) < eps) {
                g.num_shortest_paths[s][nxt] += g.num_shortest_paths[s][now];
            }
        }
    }
}

void dijkstraAllS(Graph& g) { // dijkstra with all start points
    rep(s, n) {
        dijkstra(g, s);
    }
}
