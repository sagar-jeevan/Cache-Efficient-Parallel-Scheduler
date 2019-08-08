#include "central.h"
#include <chrono>
#include <papi.h>

std::deque<Task *> myQueue;
std::stack<Sync *> sync_stack;

void increment_counter(Sync *sync)
{
    pthread_mutex_lock(&sync_lock);
    if(sync)
      sync->count = sync->count + 1;
    pthread_mutex_unlock(&sync_lock);
    return;
}
int get_counter(Sync *sync)
{
    int ret;
    pthread_mutex_lock(&sync_lock);
    if(sync)
      ret = sync->count;
    pthread_mutex_unlock(&sync_lock);
    return ret;
}

void matmul_base(int x_i, int x_j, int y_i, int y_j, int z_i, int z_j, Sync *sync)
{
    int i, j, k;
    for(i = 0; i < grain_size; i++) {
      for(k = 0; k < grain_size; k++) {
        for(j = 0; j < grain_size; j++) {
                Z[z_i + i][z_j + j] = Z[z_i + i][z_j + j] + (X[x_i + i][x_j + k] * Y[y_i + k][y_j + j]);
            }
        }
    }

    increment_counter(sync);
    if(sync && get_counter(sync) == 4) {
        Sync* current_sync = sync;
        while(current_sync != NULL) {
            if(current_sync && current_sync->flag == 0 && get_counter(current_sync) == 4) {
                Sync *new_sync = init_sync(current_sync->parent, 1,
                                           current_sync->x_i, current_sync->x_j,
                                           current_sync->y_i, current_sync->y_j,
                                           current_sync->z_i, current_sync->z_j,
                                           current_sync->n);


                matrixmul_2(new_sync->x_i, new_sync->x_j,
                            new_sync->y_i, new_sync->y_j,
                            new_sync->z_i, new_sync->z_j,
                            new_sync->n, new_sync);
                break;
            }
            else if(current_sync && current_sync->flag == 1 && get_counter(current_sync) == 4) {
                current_sync = current_sync->parent;
                increment_counter(current_sync);
            }
            else {
                break;
            }
        }
    }
}

void matrixmul_1(int x_i, int x_j, int y_i, int y_j, int z_i, int z_j, int n, Sync *sync)
{
    if (n == grain_size){
        matmul_base(x_i, x_j, y_i, y_j, z_i, z_j, sync);
        return;
    }

    Sync *new_sync = init_sync(sync, 0, x_i, x_j, y_i, y_j, z_i, z_j, n);

    push_back_task(init_task(x_i, x_j, y_i, y_j, z_i, z_j, n/2, new_sync, 0));
    push_back_task(init_task(x_i, x_j, y_i, y_j + n/2, z_i, z_j + n/2, n/2, new_sync, 0));
    push_back_task(init_task(x_i + n/2, x_j, y_i, y_j, z_i + n/2, z_j, n/2, new_sync, 0));
    push_back_task(init_task(x_i + n/2, x_j, y_i, y_j + n/2, z_i + n/2, z_j + n/2, n/2, new_sync, 0));
}

void matrixmul_2(int x_i, int x_j, int y_i, int y_j, int z_i, int z_j, int n, Sync *sync)
{

    push_back_task(init_task(x_i, x_j + n/2, y_i + n/2, y_j, z_i, z_j, n/2, sync, 1));
    push_back_task(init_task(x_i, x_j + n/2, y_i + n/2, y_j + n/2, z_i, z_j + n/2, n/2, sync, 1));
    push_back_task(init_task(x_i + n/2, x_j + n/2, y_i + n/2, y_j, z_i + n/2, z_j, n/2, sync, 1));
    push_back_task(init_task(x_i + n/2, x_j + n/2, y_i + n/2, y_j + n/2, z_i + n/2, z_j + n/2, n/2, sync, 1));
}

Sync* init_sync(Sync *parent, int flag, int x_i, int x_j, int y_i, int y_j, int z_i, int z_j, int n) {
    Sync *new_sync = new Sync;

    new_sync->flag = flag;
    new_sync->count = 0;
    new_sync->x_i = x_i;
    new_sync->x_j = x_j;
    new_sync->y_i = y_i;
    new_sync->y_j = y_j;
    new_sync->z_i = z_i;
    new_sync->z_j = z_j;
    new_sync->n = n;
    new_sync->parent = parent;

    return new_sync;
}


Task* init_task(int x_i, int x_j, int y_i, int y_j, int z_i, int z_j, int n, Sync* sync, int flag)
{
    Task *new_task = new Task;
//    new_task->fnc_ptr = &matrixmul_1;


    new_task->fnc_para.n = n;
    new_task->fnc_para.x_i = x_i;
    new_task->fnc_para.x_j = x_j;
    new_task->fnc_para.y_i = y_i;
    new_task->fnc_para.y_j = y_j;
    new_task->fnc_para.z_i = z_i;
    new_task->fnc_para.z_j = z_j;
    new_task->fnc_para.sync = sync;

    return new_task;
}
void push_sync(Sync* sync){
  pthread_mutex_lock(&sync_lock);
  sync_stack.push(sync);
  pthread_mutex_unlock(&sync_lock);
}

void push_back_task(Task* to_push)
{
  pthread_mutex_lock(&m);
  myQueue.push_back(to_push);
  pthread_mutex_unlock(&m);
}
void push_back_count(int cnt){
  pthread_mutex_lock(&vec_lock);
  count_vec.push_back(cnt);
  pthread_mutex_unlock(&vec_lock);
}
void pop_front_task()
{
  pthread_mutex_lock(&m);;
  myQueue.pop_front();
  pthread_mutex_unlock(&m);

}
void pop_back_task()
{
  pthread_mutex_lock(&m);
  myQueue.pop_back();
  pthread_mutex_unlock(&m);
}

void push_front_task(Task* to_push)
{
  pthread_mutex_lock(&m);
  myQueue.push_front(to_push);
  pthread_mutex_unlock(&m);
}

Task* get_front_task()
{
  Task *new_task;
  pthread_mutex_lock(&m);
  new_task = myQueue.front();
  pthread_mutex_unlock(&m);

  return new_task;
}

Task* get_back_task()
{
  Task *new_task;
  pthread_mutex_lock(&m);
  if(myQueue.empty()){
    pthread_mutex_unlock(&m);
    return NULL;
  }
  new_task = myQueue.back();
  myQueue.pop_back();
  pthread_mutex_unlock(&m);

  return new_task;
}

void incr_count(int *count)
{
  pthread_mutex_lock(&count_lock);
  *count= *count + 1;
  pthread_mutex_unlock(&count_lock);
}

void run_thread()
{
  while(!(myQueue).empty()) {

    Task *new_task = get_back_task();

    if(new_task) {
        matrixmul_1(new_task->fnc_para.x_i, new_task->fnc_para.x_j,
                               new_task->fnc_para.y_i, new_task->fnc_para.y_j,
                               new_task->fnc_para.z_i, new_task->fnc_para.z_j,
                               new_task->fnc_para.n, new_task->fnc_para.sync);

    }
    else
        return;
  }
}

int main(int argc, char *argv[])
{
    int k = 11;
    int i, j, n = pow(2, k);

   long long counters[6];
	int PAPI_events[] = {
		PAPI_L1_TCM,
		PAPI_L1_TCA,
		PAPI_L2_TCM,
		PAPI_L2_TCA,
		PAPI_L3_TCM,
		PAPI_L3_TCA,
		 };

	PAPI_library_init(PAPI_VER_CURRENT);


    pthread_mutex_init(&m, NULL);
    pthread_mutex_init(&work_lock, NULL);
    pthread_mutex_init(&sync_lock, NULL);


    X.resize(n);
    Y.resize(n);
    Z.resize(n);

    for(i = 0 ; i < n ; ++i) {
	X[i].resize(n);
	Y[i].resize(n);
	Z[i].resize(n);
    }

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			X[i][j] = rand() % 10;
			Y[i][j] = rand() % 10;
		}
	}
	//__cilkrts_end_cilk(); 
//	__cilkrts_set_param("nworkers","10");
	cout << "generated matrices\n";
	int numWorkers =__cilkrts_get_nworkers();
	cout << "NumWorkers:" << numWorkers << endl;
	i = PAPI_start_counters( PAPI_events, 6 );
	auto start = std::chrono::high_resolution_clock::now();
	push_back_task(init_task(0, 0, 0, 0, 0, 0, n, NULL, 0));

	for(i = 0; i < num_threads; ++i)
		cilk_spawn run_thread();
	run_thread();
	cilk_sync;
	numWorkers =__cilkrts_get_nworkers();
	cout << "NumWorkers:" << numWorkers << endl;
	auto finish = std::chrono::high_resolution_clock::now();
	PAPI_read_counters( counters, 6 );
	std::chrono::duration<double> elapsed = finish - start;
	std::cout << "Elapsed time: " << elapsed.count() << " s\n";
	cout << "L1_TCM:" << counters[0] << "\nL1_TCA:" << counters[1] << "\nL2_TCM:" << counters[2] << "\nL2_TCA:" << counters[3] << "\nL3_TCM:" << counters[4] << "\nL3_TCA:" << counters[5] << endl;
	float totalFloatOP = 2 * pow(n, 3);
	float runTime = elapsed.count();
	float FLOPS = totalFloatOP / runTime;
	float GFLOPS = FLOPS / pow(10, 9);
	float L1_MISS_RATE = ((float)counters[0] / (float)counters[1]) * 100;
	float L2_MISS_RATE = float((float)counters[2] / (float)counters[3]);
	
float L3_MISS_RATE = ((float)counters[4] / (float)counters[5]) * 100;
	cout << "L1_cache miss rate:" << L1_MISS_RATE << "% \n L2_cache miss rate:" << L2_MISS_RATE << "% \n L3_cache miss rate:" << L3_MISS_RATE << endl;
	cout << "Rate of Execution (GFLOPS): " << GFLOPS <<endl;	

	return 0;
}
