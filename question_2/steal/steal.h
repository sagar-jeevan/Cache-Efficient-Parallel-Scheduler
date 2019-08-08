#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <deque>
#include <cstdlib>
#include <cilk/cilk.h>
#include <random>
#include <pthread.h>
#include <ctime>
#include <chrono>
#include <algorithm>
#include<cilk/cilk_api.h>
#include <stack>
using namespace std;
#define grain_size 64
#define num_threads 40

pthread_mutex_t m;
pthread_mutex_t vec_lock;
pthread_mutex_t count_lock;
pthread_mutex_t sync_lock;
pthread_mutex_t work_lock;
pthread_mutex_t push_lock;
pthread_mutex_t done_lock;
pthread_mutex_t mul_mat_lock;
pthread_mutex_t mul_mat_lock2;
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

typedef void (f_ptr)(int, int, int, int, int, int, int, Sync, int);
typedef struct {
    f_ptr fnc_ptr;
    f_para fnc_para;
    f_ptr next_ptr;
    Sync* sync;
} Task;
int get_ref_cnt(Sync *sync);
void matmul_base(int x_i, int x_j, int y_i, int y_j, int z_i, int z_j, Sync*, int id);
Task* init_task(int x_i, int x_j, int y_i, int y_j, int z_i, int z_j, int n, Sync* sync, int flag);
void matrixmul_1(int x_i, int x_j, int y_i, int y_j, int z_i, int z_j, int n, Sync* sync, int id);
void matrixmul_2(int x_i, int x_j, int y_i, int y_j, int z_i, int z_j, int n, Sync* sync, int id);
void push_back_task(Task* to_push, int);
void pop_front_task(int);
void pop_back_task(int);
void push_front_task(Task* to_push, int);
Task* get_front_task(int);
Task* get_back_task(int);
void run_thread(int);
void increment_counter(Sync* sync);
void pop_sync();
void push_sync(Sync* sync);
Sync* init_sync(Sync*, int, int, int, int, int, int, int, int);
int random(int);
