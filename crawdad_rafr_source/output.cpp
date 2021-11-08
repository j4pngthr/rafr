#include"main.hpp"
#include"init.hpp"

vector<string> method_str({ "DEGREE", "BETWEENNESS", "RAFR" });

string getFileName(string file_head, int method_id) {
  string s = file_head + method_str[method_id] + ".txt";
  return s;
}

void outputIndex(const vector<int>& v) {
  int n = sz(v);
  rep(i, n) if (v[i]) cerr << i << " "; cerr << endl;
}

void outputAvailability(const vector<double>& ava, const int num_methods, const int row) {
  rep(method_id, num_methods) {
    string filename = getFileName(getFileHead() + "ava", method_id);
    if (row == 0) {
      ofstream ofs(filename);
      ofs << x_p[row] << " " << ava[method_id] << endl;
    } else {
      ofstream ofs(filename, ios::app);
      ofs << x_p[row] << " " << ava[method_id] << endl;
    }
  }
}

void outputAveDelay(const int end_ut, const int num_methods, const int row, const vector<vector<int> >& success_rate) {
  rep(method_id, num_methods) {
    string filename = getFileName(getFileHead() + "ave_del", method_id);

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
void outputDelRate(const int end_ut, const int num_methods, const int num_simu, const int row, const vector<vector<int> >& success_rate) {
  rep(method_id, num_methods) {
    string filename = getFileName(getFileHead() + "del_rat", method_id);

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

void outputDataToFile(vector<double>& ava, const int end_ut, const int num_methods, const int num_simu, const int row, vector<vector<int> >& success_rate) {
  outputAvailability(ava, num_methods, row);

  // must before accumulation
  outputAveDelay(end_ut, num_methods, row, success_rate);

  // accumulation
  rep(method_id, num_methods) rep3(j, 1, end_ut) success_rate[method_id][j] += success_rate[method_id][j - 1];

  // after the accumulation
  outputDelRate(end_ut, num_methods, num_simu, row, success_rate);

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
