#include"global.hpp"

string getFileName(string file_head, int method_id) {
  string s = file_head + method_str[method_id] + ".txt";
  return s;
}

void outputIndex(const vector<int>& v) {
  int n = sz(v);
  rep(i, n) if (v[i]) cerr << i << " "; cerr << endl;
}

void outputAvailability(const vector<double>& ava, const int row) {
  rep(method_id, num_methods) {
    string filename = getFileName(getFileHead() + "ava", method_id);
    ofstream ofs(filename, (row == 0 ? ios::out : ios::app));
    ofs << x_p[row] << " " << ava[method_id] << endl;
  }
}

void outputAveDelay(const int row, const vector<vector<int> >& success_rate) {
  rep(method_id, num_methods) {
    string filename = getFileName(getFileHead() + "ave_del", method_id);
    cerr << filename << endl;

    double ave_delay = 0;
    rep(j, end_ut) { // ave_delayを計算
      ave_delay += j * success_rate[method_id][j]; // s_cの計算時n_sで割られてる
    }
    double num_suc = 1.0 * accumulate(all(success_rate[method_id]), 0);
    ave_delay /= num_suc;

    ofstream ofs(filename, (row == 0 ? ios::out : ios::app));
    ofs << x_p[row] << " " << ave_delay << endl;
  }
}

// DelRate = end_ut-1におけるsuccess_rate
void outputDelRate(const int row, const vector<vector<int> >& success_rate) {
  rep(method_id, num_methods) {
    string filename = getFileName(getFileHead() + "del_rat", method_id);

    double del_rate = success_rate[method_id][end_ut - 1];
    del_rate /= num_simu;

    ofstream ofs(filename, (row == 0 ? ios::out : ios::app));
    ofs << x_p[row] << " " << del_rate << endl;
  }
}

void outputDataToFile(vector<double>& ava, const int row, vector<vector<int> >& success_rate) {
  outputAvailability(ava, row);

  // must before accumulation
  outputAveDelay(row, success_rate);

  // accumulation
  rep(method_id, num_methods) rep3(j, 1, end_ut) success_rate[method_id][j] += success_rate[method_id][j - 1];

  // after the accumulation
  outputDelRate(row, success_rate);

  // success rate for each time
  rep(method_id, num_methods) {
    string filename = getFileName("data/success_rate", method_id);
    ofstream ofs(filename);

    rep(i, sz(xs)) {
      ofs << xs[i] << " " << success_rate[method_id][xs[i]] * 1.0 / num_simu << endl;
    }
    ofs.close();
  }
}

void outputMetricScore(const Method &mt) { // キャッシングノード数, 候補ノード数固定
    string filename = "data/metric_value.txt";
    vector<double> vd;
    if (simu_id == 0) {
        vd = vector<double>(n, 0);
    } else {
        ifstream ifs(filename);
        string line;
        while (getline(ifs, line)) {
            istringstream iss(line);
            string str;
            while (iss >> str) {
                double x = stod(str);
                vd.eb(x);
            }
        }
    }
    if (sz(vd) != n) {
        cerr << sz(vd) << endl;
        exit(0);
    }

    rep(i, n) vd[mt.score[i].S] += mt.score[i].F / num_simu;

    ofstream ofs(filename, ios::out);
    rep(i, n) ofs << vd[i] << endl;
}
