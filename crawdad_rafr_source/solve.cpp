#include"global.hpp"

#include"maniGraph.hpp"
#include"method.hpp"
#include"output.hpp"
#include"simulation.hpp"

enum { X, REQ_BASE, REQ_RAFR, SOURCE };
int req_ctr = 0, req_ctr2 = 0, sou_ctr = 0;
int simu_id = 0;

void calcAva(const int method_id, Method mt, Method off) {
    double avail = 0;
    rep(i, n) {
        if (mt.have_data[i] && !off.have_data[i]) ++avail;
    }
    avail /= num_cache + 1; // source
    availability[method_id] += avail / num_simu;
}

Graph offNodeSelection(const Graph &g, Method &off, const int row) {
    off.alpha = 0.5;
    if (sim_pat_id == Alpha) off.alpha = x_p[row];

    callMethods(g, off, num_off, off); // オフラインノードを決定
    // cerr << "offNodeSelection/off "; outputIndex(off.have_data);

    Graph g_off(n); // オフライン→gが空
    makeOff(g, g_off, off.have_data); // maniGraph, g_offを作る
    return g_off;
}

// 1回読んですべての手法のrequest nodeを求める
void getReqNode(vector<Method> &mt, Method &off, vector<int> &req) {
    vector<int> is_forbidden(n);
    rep(i, n) {
        rep(method_id, num_methods) {
            // not source, caching, off
            if (mt[method_id].have_data[i] || off.have_data[i]) is_forbidden[i] = 1;
        }
    }
    // cerr << "is_forbidden "; rep(i, n) if (is_forbidden[i]) cerr << i << " "; cerr << endl;

    while (1) {
        int _req = rand() % n;
        if (is_forbidden[_req]) continue;
        rep(method_id, num_methods) {
            req[method_id] = _req;
        }
        break;
    }
    // { // まずは比較対象で共通のリクエスタノード
    //     [&]() {
    //         if (num_methods == 1 && vi[0] == RAFR) return;
    //
    //         int _req = rands[REQ_BASE][req_ctr++];
    //         if (sz(rands[REQ_BASE]) < req_ctr) {
    //             cerr << sz(rands[REQ_BASE]) << endl;
    //             exit(0);
    //         }
    //         assert(0 <= _req && _req < n);
    //         // cerr << "getReqNode/_req " << _req << endl;
    //
    //         while (1) { // どの比較対象の手法でも，キャッシングノードでもオフラインノードでもないの
    //             rep(method_id, num_methods) {
    //                 if (vi[method_id] == RAFR) continue;
    //
    //                 if (mt[method_id].have_data[_req] || off.have_data[_req]) {
    //                     _req = rands[REQ_BASE][req_ctr++];
    //                     assert(sz(rands[REQ_BASE]) >= req_ctr);
    //                     assert(0 <= _req && _req < n);
    //
    //                     break;
    //                 }
    //                 if (method_id == 2) {
    //                     rep(_method_id, 3) {
    //                         req[_method_id] = _req;
    //                     }
    //                     return;
    //                 }
    //             }
    //         }
    //     }();
    // }
    //
    // { // RAFC
    //     int method_id = -1;
    //     rep(_method_id, num_methods) { // method_idを求める
    //         if (vi[_method_id] == OPT || vi[_method_id] == RAFR) {
    //             method_id = _method_id;
    //             break;
    //         }
    //     }
    //
    //     int temp = -1; // 比較対象どれかのidがほしい
    //     rep(method_id, num_methods) {
    //         if (vi[method_id] != OPT && vi[method_id] != RAFR) {
    //             temp = method_id;
    //             break;
    //         }
    //     }
    //     // cerr << "getReqNode/temp " << temp << endl;
    //
    //     int _req = -1;
    //     if (temp != -1) _req = req[temp]; // まずは比較対象と同じの
    //     else {
    //         rep(i, n) {
    //             if (mt[method_id].have_data[i] || off.have_data[i]) continue;
    //             _req = i;
    //             break;
    //         }
    //     }
    //
    //     while (1) {
    //         if (mt[method_id].have_data[_req] || off.have_data[_req]) {
    //             _req = rands[REQ_RAFR][req_ctr2++];
    //             assert(sz(rands[REQ_BASE]) >= req_ctr2);
    //             assert(0 <= _req && _req < n);
    //         } else {
    //             req[method_id] = _req;
    //             break;
    //         }
    //     }
    // }
}

void solve(const vector<vector<pii> >& contact_nodes, const Graph& g, const int row) {
    cerr << "cache off " << num_cache << " " << num_off << endl;
    // cerr << "g.size "; rep(i, n) cerr << g.g[i].size() << " "; cerr << endl;
    for (simu_id = 0; simu_id < num_simu; ++simu_id) {
        if (simu_id % 100 == 0) cerr << "row " << row << " simu_id " << simu_id << " (solve.cpp)" << endl;

        Method off(0, 1, DEGREE, n); // オフラインノード
        Graph g_off = offNodeSelection(g, off, row);

        // show off nodes
        // cerr << "off ";
        // rep(i, n) {
        //     if (off.have_data[i]) cerr << i << " ";
        // } cerr << endl;

        // cerr << "g_off/g.size "; rep(i, n) cerr << g_off.g[i].size() << " "; cerr << endl;

        // rep(i, n) {
        //     cerr << i << "    ";
        //     for (int j : g_off.g[i]) cerr << j << " ";
        //     cerr << endl;
        // }

        vector<Method> mt(num_methods);
        int source = -1;
        {
            // not off
            do {
                source = rand() % n;
                // cerr << "source " << source << " " << off.have_data[source] << endl;
            } while (off.have_data[source]);
            // cerr << "source " << source << endl;

            // cerr << "source selected" << endl;

            rep(method_id, num_methods) {
                mt[method_id] = Method((vi[method_id] == RAFR ? 1 : 0), 0, vi[method_id], n);
                // cerr << mt[method_id].have_data.size() << endl;
                mt[method_id].have_data[source] = 1;
            }
            // cerr << "source is set" << endl;
        }

        rep(method_id, num_methods) {
            // mt[method_id] = Method((vi[method_id] == RAFR ? 1 : 0), 0, vi[method_id], n);
            if (vi[method_id] == RAFR) {
                mt[method_id].cand = n / 2;
                if (sim_pat_id == Candidate) mt[method_id].cand = (int)(0.01 * x_p[row] * n);
            }

            // { // ソースノード, 共通じゃないのか
            //     int source = rands[SOURCE][sou_ctr++];
            //     assert(sou_ctr <= sz(rands[SOURCE]));
            //     while (off.have_data[source]) {
            //         source = rands[SOURCE][sou_ctr++];
            //     }
            //     mt[method_id].have_data[source] = 1;
            //     // cerr << "source " << source << endl;
            // }

            if (vi[method_id] != OPT) callMethods(g, mt[method_id], num_cache, off); // キャッシングノードを求める, スコアも計算
            else {
                // cerr << "OPT" << endl;
                callMethods(g_off, mt[method_id], num_cache, off);
                // ソースノードだけオフラインになりうる
            }

            // show caching nodes
            // cerr << method_str[method_id] << " ";
            // rep(i, n) if (mt[method_id].have_data[i]) cerr << i << " "; cerr << endl;

            calcAva(method_id, mt[method_id], off);
            // cerr << "ava is determined" << endl;
        }

        vector<int> req(num_methods);
        getReqNode(mt, off, req);
        // cerr << "req " << req << endl;

        rep(method_id, num_methods) {
            assert(0 <= req[method_id] && req[method_id] < n);

            int success_ut = simulation(contact_nodes, g_off, mt[method_id].have_data, off.have_data, req[method_id], method_id);
            if (success_ut >= 0) {
                ++success_rate[method_id][success_ut];
            }
        }
    }
}
