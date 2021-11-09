#include"global.hpp"

void connect(Graph& g, int node1, int node2) {
  rep(i, 2) {
    swap(node1, node2);
    g.g[node1].emplace(node2);
    ++g.lambda[node1][node2];
  }
}

// 準備期間あり
void makeContacts(vector<vector<pii> >& contact_nodes, const int contact_ut, int& end_ut, Graph& g, int node1, int node2, const int pre_end_ut, int& start_ut) {
  if (contact_ut < pre_end_ut) {
    connect(g, node1, node2);
  } else {
    int ut = contact_ut - pre_end_ut;
    chmin(start_ut, ut - 2);
    chmax(end_ut, ut);
    contact_nodes[ut].eb(node1, node2);
  }
}

void getRealTrace(vector<vector<pii> >& contact_nodes, int& end_ut, Graph& g, int& start_ut) {
  int n = g.n;

  int pre_end_ut = 0, first_contact_ut = inf;

  string str = "tr_iMotes/contacts.Exp"; // 入力ファイル
  // str += (simu_id < 3 ? to_string(simu_id + 1) : "6");
  str += "6";
  str += ".txt";
  ifstream ifs(str);
  string line;
  vector<vector<int> > v; // ノードとコンタクト時間

  while (getline(ifs, line)) {
    istringstream stream(line);
    vector<int> v2;
    while (getline(stream, str, ' ')) {
      if (str[0] >= '0' && str[0] <= '9') {
        v2.eb(stoi(str));
      }
    }

    --v2[0]; --v2[1];
    int node1 = v2[0], node2 = v2[1], contact_ut= v2[2];
    if (node1 == node2 || node1 < start_vld_id || node1 > end_vld_id || node2 < start_vld_id || node2 > end_vld_id) continue;

    chmax(pre_end_ut, contact_ut);
    chmin(first_contact_ut, contact_ut);

    v2[0] -= start_vld_id; v2[1] -= end_vld_id;
    v.eb(v2);
  }

  cerr << "first end " << first_contact_ut << " " << pre_end_ut << endl;
  pre_end_ut = first_contact_ut + (pre_end_ut - first_contact_ut) / 2;
  cerr << "pre_end_ut " << pre_end_ut << endl;
  // pre_end_ut = 100000;
  // cerr << sz(v) << endl;
  rep(i, sz(v)) {
    int node1 = v[i][0], node2 = v[i][1], contact_ut = v[i][2];
    // end_ut=0の必要
    makeContacts(contact_nodes, contact_ut, end_ut, g, node1, node2, pre_end_ut, start_ut);
  }

  rep(i, n) rep(j, i) { // j < i
    g.lambda[i][j] /= pre_end_ut - first_contact_ut; // 接触頻度
  }
  rep(i, n) rep(j, i) g.lambda[j][i] = g.lambda[i][j];

  // rep(i, 1) {
  //   cerr << i << "    ";
  //   for (int a : g.g[i]) cerr << a << " "; cerr << endl;
  // }
  // cerr << "simu_id " << simu_id << endl; rep(i, n) cerr << sz(g.g[i]) << " "; cerr << endl;
}
