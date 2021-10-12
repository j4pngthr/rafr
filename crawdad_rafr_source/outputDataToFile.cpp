#include"main.hpp"
#include"outputDataToFile.hpp"

// vector<int> xs({ 1000, 5000, 10000, 50000, 100000 });
// vector<string> vs({ "DEGREE", "BETWEENNESS", "RAFR" });
// vector<double> x_p({ 1.28, 2.56, 3.85, 5.13, 6.41, 7.69, 8.97 });
//
// void outputAvailability(const vector<double>& ava, const string file_head, const int num_methods, const int row) {
//   rep(method_id, num_methods) {
//     string filename = file_head + "ava" + vs[method_id] + ".txt";
//     if (row == 0) {
//       ofstream ofs(filename);
//       ofs << x_p[row] << " " << ava[method_id] << endl;
//     } else {
//       ofstream ofs(filename, ios::app);
//       ofs << x_p[row] << " " << ava[method_id] << endl;
//     }
//   }
// }
//
// void outputAveDelay(const int end_ut, const string file_head, const int num_methods, const int row, const vector<vector<int> >& success_rate) {
//   rep(method_id, num_methods) {
//     string filename = file_head + "ave_del" + vs[method_id] + ".txt";
//
//     int ave_delay = 0;
//     rep(j, end_ut) { // ave_delayを計算
//       ave_delay += j * success_rate[method_id][j]; // s_cの計算時n_sで割られてる
//     }
//
//     if (row == 0) {
//       ofstream ofs(filename);
//       ofs << x_p[row] << " " << ave_delay * 1.0 / accumulate(all(success_rate[method_id]), 0) << endl;
//     } else {
//       ofstream ofs(filename, ios::app);
//       ofs << x_p[row] << " " << ave_delay * 1.0 / accumulate(all(success_rate[method_id]), 0) << endl;
//     }
//   }
// }
//
// // end_ut-1におけるsuccess_rate
// void outputDelRate(const int end_ut, const string file_head, const int num_methods, const int num_simu, const int row, const vector<vector<int> >& success_rate) {
//   rep(method_id, num_methods) {
//     string filename = file_head + "del_rat" + vs[method_id] + ".txt";
//
//     if (row == 0) {
//       ofstream ofs(filename);
//       ofs << x_p[row] << " " << success_rate[method_id][end_ut - 1] * 1.0 / num_simu << endl;
//     } else {
//       ofstream ofs(filename, ios::app);
//       ofs << x_p[row] << " " << success_rate[method_id][end_ut - 1] * 1.0 / num_simu << endl;
//     }
//   }
// }
//
// void outputDataToFile(vector<double>& ava, const int end_ut, const string file_head, const int num_methods, const int num_simu, const int row, vector<vector<int> >& success_rate) {
//   outputAvailability(ava, file_head, num_methods, row);
//   // before the accumulation
//   outputAveDelay(end_ut, file_head, num_methods, row, success_rate);
//   // accumulation
//   rep(method_id, num_methods) rep3(j, 1, end_ut) success_rate[method_id][j] += success_rate[method_id][j - 1];
//   // after the accumulation
//   outputDelRate(end_ut, file_head, num_methods, num_simu, row, success_rate);
//
//   rep(method_id, num_methods) {
//     string filename = "data/success_rate" + vs[method_id] + ".txt";
//     ofstream ofs(filename);
//
//     rep(i, sz(xs)) {
//       ofs << xs[i] << " " << success_rate[method_id][xs[i]] * 1.0 / num_simu << endl;
//     }
//     ofs.close();
//   }
// }

// class outputDataToFile {
// private:
//   int end_ut;
//   int num_methods;
//   int num_simu;
//   int row;
//   vector<double>& ava;
//   vector<vector<int> >& success_rate;
//
//   vector<int> xs;
//   vector<string> vs;
//   vector<double> x_p;
//
//   void outputAvailability() {
//     rep(method_id, num_methods) {
//       string filename = "data/ava" + vs[method_id] + ".txt";
//
//       if (row == 0) {
//         ofstream ofs(filename);
//         ofs << x_p[row] << " " << ava[method_id] << endl;
//       } else {
//         ofstream ofs(filename, ios::app);
//         ofs << x_p[row] << " " << ava[method_id] << endl;
//       }
//     }
//   }
//
//   void outputAveDelay() {
//     rep(method_id, num_methods) {
//       string filename = "data/average_delay" + vs[method_id] + ".txt";
//
//       int ave_delay = 0;
//       rep(j, end_ut) { // ave_delayを計算
//         ave_delay += j * success_rate[method_id][j]; // s_cの計算時n_sで割られてる
//       }
//
//       if (row == 0) {
//         ofstream ofs(filename);
//         ofs << x_p[row] << " " << ave_delay * 1.0 / accumulate(all(success_rate[method_id]), 0) << endl;
//       } else {
//         ofstream ofs(filename, ios::app);
//         ofs << x_p[row] << " " << ave_delay * 1.0 / accumulate(all(success_rate[method_id]), 0) << endl;
//       }
//     }
//   }
//
//   // end_ut-1におけるsuccess_rate
//   void outputDelRate() {
//     rep(method_id, num_methods) {
//       string filename = "data/delivery_rate" + vs[method_id] + ".txt";
//
//       if (row == 0) {
//         ofstream ofs(filename);
//         ofs << x_p[row] << " " << success_rate[method_id][end_ut - 1] * 1.0 / num_simu << endl;
//       } else {
//         ofstream ofs(filename, ios::app);
//         ofs << x_p[row] << " " << success_rate[method_id][end_ut - 1] * 1.0 / num_simu << endl;
//       }
//     }
//   }
// public:
//   // constで受け取れない？
//   outputDataToFile(vector<double>& ava, int end_ut, int num_metods, int num_simu, int row,
//     vector<vector<int> >& success_rate) : ava(ava), end_ut(end_ut), num_methods(num_methods), num_simu(num_simu), row(row), success_rate(success_rate) {
//     // init
//     xs = vector<int>({ 1000, 5000, 10000, 50000, 100000 });
//     vs = vector<string>({ "DEGREE", "BETWEENNESS", "RAFR" });
//     x_p = vector<double>({ 1.28, 2.56, 3.85, 5.13, 6.41, 7.69, 8.97 });
//     // 累積和
//     rep(method_id, num_methods) rep3(j, 1, end_ut) success_rate[method_id][j] += success_rate[method_id][j - 1];
//
//     rep(method_id, num_methods) {
//       string filename = "data/success_rate" + vs[method_id] + ".txt";
//       ofstream ofs(filename);
//
//       rep(i, sz(xs)) {
//         ofs << xs[i] << " " << success_rate[method_id][xs[i]] * 1.0 / num_simu << endl;
//       }
//       ofs.close();
//     }
//   }
// };

// class outputDataToFile {
// private:
//   int end_ut;
//   vector<int> xs;
//
//   void outputAvailability() {}
// public:
//   outputDataToFile(int end_ut) : end_ut(end_ut) {
//     // init
//     xs = vector<int>({ 1000, 5000, 10000, 50000, 100000 });
//   }
// };
