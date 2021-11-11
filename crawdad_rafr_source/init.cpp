#include"global.hpp"

#include"error.hpp"

string getSimPatStr() {
  string s = "";
  if (sim_pat_id == Offline) s = "off";
  else if (sim_pat_id == Caching) s = "cache";
  else if (sim_pat_id == Candidate) s = "candidate";
  else if (sim_pat_id == Alpha) s = "alpha";
  else error("incorrect sim_pat_id (init.cpp)");
  return s;
}

void initFileHead() {
  cerr << getSimPatStr() << endl;
  setFileHead(getFileHead() + getSimPatStr() + "/");
  cerr << getFileHead() << endl;
}

void initVecSize() {
  success_rate = vector<vector<int> >(num_methods, vector<int>(600000, 0));
  availability = vector<double>(num_methods, 0);
}

void initVldId() {
  start_vld_id = 20, end_vld_id = 97;
  n = end_vld_id - start_vld_id + 1;
}

void initXp() {
  if (sim_pat_id == Offline || sim_pat_id == Caching) {
    x_p = vector<double>({ 1.28, 2.56, 3.85, 5.13, 6.41, 7.69, 8.97 });
  } else if (sim_pat_id == Candidate) {
    x_p = vector<double>({ 20, 30, 40, 50, 60, 70, 80 });
  } else if (sim_pat_id == Alpha) {
    x_p = vector<double>({ 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0 });
  } else {
    error("incorrect sim_pat_id (init.cpp)");
  }
}

void init() {
  initFileHead();
  initVecSize();
  initVldId();
  initXp();
}
