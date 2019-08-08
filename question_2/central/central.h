#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <sstream>
#include <string>
#include <deque>
#include <cstdlib>
#include <cilk/cilk.h>
#include <pthread.h>
#include<cilk/cilk_api.h>
#include <stack>
using namespace std;
#define grain_size 2
#define num_threads 48

pthread_mutex_t m;
pthread_mutex_t work_lock;
pthread_mutex_t vec_lock;
pthread_mutex_t count_lock;
pthread_mutex_t sync_lock;
std::vector< std::vector<int> > X;
std::vector< std::vector<int> > Y;
std::vector< std::vector<int> > Z;
std:: vector <int> count_vec;

typedef struct sync {
    int flag;
    int count;
    int x_i;
    int x_j;
    int y_i;
    int y_j;
    int z_i;
    int z_j;
    int n;
    struct sync *parent;
} Sync;
typedef struct {
  int x_i;
  int x_j;
  int y_i;
  int y_j;
  int z_i;
  int z_j;
  int n;
  Sync* sync;
} f_para;

typedef void (f_ptr)(int, int, int, int, int, int, int, Sync);
typedef struct {
    f_ptr fnc_ptr;
    f_para fnc_para;
    f_ptr next_ptr;
    Sync* sync;
} Task;
int get_ref_cnt(Sync *sync);
void matmul_base(int x_i, int x_j, int y_i, int y_j, int z_i, int z_j, Sync*);
Task* init_task(int x_i, int x_j, int y_i, int y_j, int z_i, int z_j, int n, Sync* sync, int flag);
void matrixmul_1(int x_i, int x_j, int y_i, int y_j, int z_i, int z_j, int n, Sync* sync );
void matrixmul_2(int x_i, int x_j, int y_i, int y_j, int z_i, int z_j, int n, Sync* sync);
void push_back_task(Task* to_push);
void pop_front_task();
void pop_back_task();
void push_front_task(Task* to_push);
void push_back_count(int cnt);
Task* get_front_task();
Task* get_back_task();
void run_thread();
void increment_counter(Sync* sync);
void pop_sync(Sync* sync);
void push_sync(Sync* sync);
Sync* init_sync(Sync*, int, int, int, int, int, int, int, int);
