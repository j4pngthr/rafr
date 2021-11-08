#ifndef macro_h
#define macro_h

#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

#define F first
#define S second
#define eb emplace_back
#define all(v) v.begin(), v.end()
#define rep(i, n) for (int i = 0; i < (n); ++i)
#define rep3(i, l, n) for (int i = l; i < (n); ++i)
#define sz(v) (int)v.size()
#define endl '\n'
#define abs(x) (x >= 0 ? x : -(x))
#define lb(v, x) (int)(lower_bound(all(v), x) - v.begin())
#define ub(v, x) (int)(upper_bound(all(v), x) - v.begin())
#define eps 1e-8
// #define inf 1000000000
#define e 2.71828

#define BETWEENNESS 14
#define CLOSENESS 211
#define CONNECTIVITY 214
#define DEGREE 34
#define RAFR 1705
#define RANDOM 17013

template<typename T1, typename T2> inline bool chmin(T1 &a, T2 b) { if (a > b) { a = b; return 1; } return 0; }
template<typename T1, typename T2> inline bool chmax(T1 &a, T2 b) { if (a < b) { a = b; return 1; } return 0; }
template<typename T, typename U> T pow_(T a, U b) { return b ? pow_(a * a, b / 2) * (b % 2 ? a : 1) : 1; }
template<class T, class U> ostream& operator << (ostream& os, const pair<T, U>& p) { os << p.F << " " << p.S; return os; }
template<class T, class U> void operator += (pair<T, U>& p, const pair<T, U>& q) { p.F += q.F; p.S += q.S; }
template<class T> ostream& operator << (ostream& os, const vector<T>& vec) { rep(i, sz(vec)) { if (i) os << " "; os << vec[i]; } return os; }
template<typename T> inline istream& operator >> (istream& is, vector<T>& v) { rep(j, sz(v)) is >> v[j]; return is; }
template<class T> void operator += (vector<T>& v, vector<T>& v2) { rep(i, sz(v2)) v.eb(v2[i]); }
template<class T> ostream& operator << (ostream& os, const set<T>& s) { for (auto si : s) os << si << " "; return os; }

using pii = pair<int, int>;
using pdi = pair<double, int>;
using pdd = pair<double, double>;
using pip = pair<int, pii>;

const int inf = numeric_limits<int>::max();
const double dinf = numeric_limits<double>::infinity();



class Graph {
public:
  int n;
  vector<vector<double> > d;
  vector<set<int> > g;
  vector<vector<int> > hop; // ijの最短経路のホップ数
  vector<vector<double> > lambda;
  vector<vector<int> > num_shortest_paths; // ijの最短経路の個数
  vector<vector<int> > parent; // iをrootとしたときのjの親
  vector<int> valid_id;

  Graph(int n, int va0, int va1) : n(n) {
    valid_id.resize(2);
    valid_id[0] = va0, valid_id[1] = va1;
    d = vector<vector<double> >(n, vector<double>(n, dinf));
    rep(i, n) d[i][i] = 0;
    g.resize(n);
    hop = num_shortest_paths = vector<vector<int> >(n, vector<int>(n, inf));
    rep(i, n) hop[i][i] = num_shortest_paths[i][i] = 0;
    lambda = vector<vector<double> >(n, vector<double>(n));
    parent = vector<vector<int> >(n, vector<int>(n, -1));
  }
};

class Method {
public:
  int is_ite, is_pro, method_id, n, cand;
  double alpha = 0;
  vector<int> have_data;
  vector<pdi> score;

  void init_score() {
    rep(i, n) {
      score[i].F = 0;
      score[i].S = i;
    }
  }
  Method(int is_ite, int is_pro, int method_id, int n) : is_ite(is_ite), is_pro(is_pro), method_id(method_id), n(n) {
    have_data.resize(n);
    score.resize(n);
    init_score();
  }
};

#endif
