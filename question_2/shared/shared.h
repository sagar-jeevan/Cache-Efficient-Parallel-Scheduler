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
#define grain_size 32
#define num_threads 4

pthread_mutex_t dequeLock[num_threads];
pthread_mutex_t workLock;
pthread_mutex_t syncLock;
pthread_mutex_t push_lock;
pthread_mutex_t done_lock;  
std::vector< std::vector<int> > X;
std::vector< std::vector<int> > Y;
std::vector< std::vector<int> > Z;
std:: vector <int> count_vec;
int work_load[num_threads] = {0};

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
    f_para functionPara;
    f_ptr next_ptr;
    Sync* sync;
} Task;
int get_ref_cnt(Sync *sync);
void MMBase(int x_i, int x_j, int y_i, int y_j, int z_i, int z_j, Sync*, int id);
Task* initTask(int x_i, int x_j, int y_i, int y_j, int z_i, int z_j, int n, Sync* sync, int flag);
void MMFirstHalf(int x_i, int x_j, int y_i, int y_j, int z_i, int z_j, int n, Sync* sync, int id);
void MMSecondHalf(int x_i, int x_j, int y_i, int y_j, int z_i, int z_j, int n, Sync* sync, int id);
void pushBackTask(Task* toPush, int);
void popFrontTask(int);
void pop_back_task(int);
void pushFrontTask(Task* toPush, int);
Task* getFrontTask(int);
Task* getBackTask(int);
void run_thread(int);
void incrementCounter(Sync* sync);
void pop_sync(Sync* sync);
void push_sync(Sync* sync);
Sync* initSync(Sync*, int, int, int, int, int, int, int, int);
int random(int);
