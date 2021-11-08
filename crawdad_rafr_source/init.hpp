#ifndef init_hpp
#define init_hpp

string getFileHead();
void init();

extern const int num_methods, num_simu, debg_flg;
extern int end_ut, num_cache, num_off, start_ut;
extern int end_vld_id, start_vld_id;
extern int n;
extern const vector<int> vi;
extern vector<int> xs;
extern vector<double> x_p;
extern int sim_pat_id;

enum sim_pat {
  Offline,
  Caching,
  Candidate,
  Alpha
};

#endif
