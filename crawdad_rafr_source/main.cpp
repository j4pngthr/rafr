#include"main.hpp"
#include"getRealTrace.hpp"
#include"maniGraph.hpp"
#include"method.hpp"
// #include"outputDataToFile.hpp"
#include"simulation.hpp"

// need to adjust file_head and row
const int num_methods = 3, num_simu = 1000, debg_flg = 0;
string file_head = "data/";
const vector<int> vi({ DEGREE, BETWEENNESS, RAFR });

vector<int> xs({ 1000, 5000, 10000, 50000, 100000 });
vector<string> vs({ "DEGREE", "BETWEENNESS", "RAFR" });

// 出力の横軸
vector<double> x_p;

int sim_pat_id;


void outputIndex(const vector<int>& v) {
  int n = sz(v);
  rep(i, n) if (v[i]) cerr << i << " "; cerr << endl;
}

void outputAvailability(const vector<double>& ava, const string file_head, const int num_methods, const int row) {
  rep(method_id, num_methods) {
    string filename = file_head + "ava" + vs[method_id] + ".txt";
    if (row == 0) {
      ofstream ofs(filename);
      ofs << x_p[row] << " " << ava[method_id] << endl;
    } else {
      ofstream ofs(filename, ios::app);
      ofs << x_p[row] << " " << ava[method_id] << endl;
    }
  }
}

void outputAveDelay(const int end_ut, const string file_head, const int num_methods, const int row, const vector<vector<int> >& success_rate) {
  rep(method_id, num_methods) {
    string filename = file_head + "ave_del" + vs[method_id] + ".txt";

    double ave_delay = 0;
    rep(j, end_ut) { // ave_delayを計算
      ave_delay += j * success_rate[method_id][j]; // s_cの計算時n_sで割られてる
    }
    double num_suc = 1.0 * accumulate(all(success_rate[method_id]), 0);
    ave_delay /= num_suc;

    if (row == 0) {
      ofstream ofs(filename);
      ofs << x_p[row] << " " << ave_delay << endl;
    } else {
      ofstream ofs(filename, ios::app);
      ofs << x_p[row] << " " << ave_delay << endl;
    }
  }
}

// DelRate = end_ut-1におけるsuccess_rate
void outputDelRate(const int end_ut, const string file_head, const int num_methods, const int num_simu, const int row, const vector<vector<int> >& success_rate) {
  rep(method_id, num_methods) {
    string filename = file_head + "del_rat" + vs[method_id] + ".txt";

    double del_rate = success_rate[method_id][end_ut - 1];
    del_rate /= num_simu;

    if (row == 0) {
      ofstream ofs(filename);
      ofs << x_p[row] << " " << del_rate << endl;
    } else {
      ofstream ofs(filename, ios::app);
      ofs << x_p[row] << " " << del_rate << endl;
    }
  }
}

void outputDataToFile(vector<double>& ava, const int end_ut, const string file_head, const int num_methods, const int num_simu, const int row, vector<vector<int> >& success_rate) {
  outputAvailability(ava, file_head, num_methods, row);

  // must before accumulation
  outputAveDelay(end_ut, file_head, num_methods, row, success_rate);

  // accumulation
  rep(method_id, num_methods) rep3(j, 1, end_ut) success_rate[method_id][j] += success_rate[method_id][j - 1];

  // after the accumulation
  outputDelRate(end_ut, file_head, num_methods, num_simu, row, success_rate);

  // success rate for each time
  rep(method_id, num_methods) {
    string filename = "data/success_rate" + vs[method_id] + ".txt";
    ofstream ofs(filename);

    rep(i, sz(xs)) {
      ofs << xs[i] << " " << success_rate[method_id][xs[i]] * 1.0 / num_simu << endl;
    }
    ofs.close();
  }
}

void solve(vector<double>& availability, const vector<vector<pii> >& contact_nodes, const int end_ut, const Graph& g, const int num_cache, const int num_off, const double row, const int start_ut, vector<vector<int> >& success_rate) {
  int n = g.n;
  rep(simu_id, num_simu) {
    if (debg_flg) cerr << "row " << row << " simu_id " << simu_id << endl;

    // delclare the offline node
    Method off(0, 1, DEGREE, n);
    // off.alpha = x_p[row];
    off.alpha = 0.5;

    // make offline nodes
    callMethods(g, off, num_off);
    if (debg_flg) { cerr << "off "; outputIndex(off.have_data); }

    // declare a graph considering offline nodes
    Graph g_off(n, g.valid_id[0], g.valid_id[1]);
    // construct the graph
    makeOff(g, g_off, off.have_data);

    rep(method_id, num_methods) {
      // declare the method obtaining caching nodes
      Method mt((vi[method_id] == RAFR ? 1 : 0), 0, vi[method_id], n);
      mt.cand = (int)(0.01 * x_p[row] * n);
      // mt.cand = n / 2;

      int source = rand() % n;
      // while (off.have_data[source]) {
      //   source = rand() % n;
      // }
      mt.have_data[source] = 1;
      if (debg_flg) cerr << "source " << source << endl;

      // obtain caching nodes based on the graph without offline nodes
      // source is not included in caching
      callMethods(g, mt, num_cache);
      if (debg_flg) { cerr << vs[method_id] << " "; outputIndex(mt.have_data); }

      double avail = 0;
      rep(i, n) {
        if (mt.have_data[i] && !off.have_data[i]) ++avail;
      }
      avail /= num_cache + 1; // source
      availability[method_id] += avail / num_simu;

      // declare a requesting node
      int req = rand() % n;
      while (mt.have_data[req] || off.have_data[req]) {
        req = rand() % n;
      }

      int success_ut = simulation(contact_nodes, end_ut, g_off, mt.have_data, off.have_data, req, start_ut);
      if (success_ut >= 0) {
        ++success_rate[method_id][success_ut];
      }
    } // rep(method_id, )
  } // rep(simu_id, )

  // outputDataToFile is called in main
}

void init_xp() {
  if (sim_pat_id == Offline || sim_pat_id == Caching) {
    x_p = vector<double>({ 1.28, 2.56, 3.85, 5.13, 6.41, 7.69, 8.97 });
  } else if (sim_pat_id == Candidate) {
    x_p = vector<double>({ 20, 30, 40, 50, 60, 70, 80 });
  } else if (sim_pat_id == Alpha) {
    x_p = vector<double>({ 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0 });
  } else {

  }
}

void init() {
  sim_pat sim_pat_id = Offline;
  init_xp();
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  cout.tie(0);
  cout << fixed << setprecision(12);

  init();
  vector<vector<pii> > contact_nodes(300000); // 時間引数
  int end_ut = 0, start_ut = inf;

  // obtain valid_ids
  vector<int> v(2);
  // v[1] = 8;
  // v[1] = 11;
  // v[1] = 40;
  v[0] = 20; v[1] = 97;
  int n = v[1] - v[0] + 1;
  // rep3(i, 2, 8) cerr << (int)(n * 0.1 * i) << " "; cerr << endl;

  // declare a graph
  Graph g(n, v[0], v[1]);
  // construct the graph
  getRealTrace(contact_nodes, end_ut, g, start_ut);
  makeDFromLambda(g);
  solveShortest(g);

  // the result is different from the case in which we use the constant num_cahce and num_off
  // since the rand is different
  int num_cache = 3, num_off = 5;
  file_head += "candidate/";

  rep(row, sz(x_p)) {
    // visualize the condition
    cerr << row << endl;
    // num_cache = row + 1; // 1 indexed

    // delaration
    vector<vector<int> > success_rate(num_methods, vector<int>(600000));
    vector<double> availability(num_methods);
    // simulate in this condition
    solve(availability, contact_nodes, end_ut, g, num_cache, num_off, row, start_ut, success_rate);
    // obtain the line number in the output
    outputDataToFile(availability, end_ut, file_head, num_methods, num_simu, row, success_rate);
  }
}
