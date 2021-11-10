#include"global.hpp"

const int num_methods = 3, num_simu = 1000, debg_flg = 0;
int end_ut = 0, num_cache = 3, num_off = 5, start_ut = inf;
int end_vld_id, start_vld_id; // valid id
int n;
static string file_head = "data/";
const vector<int> vi({ DEGREE, BETWEENNESS, RAFR });
vector<int> xs({ 1000, 5000, 10000, 50000, 100000 });
// 出力の横軸 x plot
vector<double> x_p;
// simulation pattern id
int sim_pat_id;
vector<vector<int> > success_rate;
vector<double> availability;

string getFileHead() {
  return file_head;
}

void setFileHead(string s) {
  file_head = s;
}
