#include"main.hpp"
#include"error.hpp"
#include"init.hpp"

const int num_cache = 3, num_methods = 3, num_off = 5, num_simu = 1000, debg_flg = 0;
static string file_head = "data/";
const vector<int> vi({ DEGREE, BETWEENNESS, RAFR });
vector<int> xs({ 1000, 5000, 10000, 50000, 100000 });
// 出力の横軸
vector<double> x_p;
// simulation pattern id
int sim_pat_id;
enum sim_pat {
  Offline,
  Caching,
  Candidate,
  Alpha
};

string getFileHead() {
  return getFileHead();
}

string getSimPatStr() {
  string s = "";
  if (sim_pat_id == Offline) s = "Offline";
  else if (sim_pat_id == Caching) s = "Caching";
  else if (sim_pat_id == Candidate) s = "Candidate";
  else if (sim_pat_id == Alpha) s = "Alpha";
  else error("incorrect sim_pat_id (init.cpp)");
  return s;
}

void initFileHead() {
  file_head += getSimPatStr() + "/";
}

void initXp() {
  if (sim_pat_id == Offline || sim_pat_id == Caching) {
    x_p = vector<double>({ 1.28, 2.56, 3.85, 5.13, 6.41, 7.69, 8.97 });
  } else if (sim_pat_id == Candidate) {
    x_p = vector<double>({ 20, 30, 40, 50, 60, 70, 80 });
  } else if (sim_pat_id == Alpha) {
    x_p = vector<double>({ 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0 });
  } else {
    error("sim_pat_id is incorrect");
  }
}

void init() {
  sim_pat sim_pat_id = Offline;
  initXp();
  initFileHead();
}
