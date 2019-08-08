#include "stealMod.h"
#include <chrono>

std::deque<Task*> myQueue[num_threads];
std::stack<Sync *> sync_stack;
std::vector<Task *> pool;

int randomNumber(int id){
    int randomNumber = id;

    while (randomNumber == id)
        randomNumber = rand() % num_threads;

    return randomNumber;
}

void incrementCounter(Sync *sync)
{
    pthread_mutex_lock(&syncLock);
    if(sync)
        sync->count = sync->count + 1;
    pthread_mutex_unlock(&syncLock);

    return;
}

int getCounter(Sync *sync)
{
    int returnValue;

    pthread_mutex_lock(&syncLock);
    if(sync)
        returnValue = sync->count;
    pthread_mutex_unlock(&syncLock);

    return returnValue;
}

void MMBase(int x_i, int x_j, int y_i, int y_j, int z_i, int z_j, Sync *sync, int id)
{
    int i, j, k;

    for(i = 0; i < grain_size; i++)
        for(k = 0; k < grain_size; k++)
            for(j = 0; j < grain_size; j++)
                Z[z_i + i][z_j + j] = Z[z_i + i][z_j + j] + (X[x_i + i][x_j + k] * Y[y_i + k][y_j + j]);

    incrementCounter(sync);
    if(sync && getCounter(sync) == 4) {
        Sync* currentSync = sync;
        while(currentSync != NULL) {
            if(currentSync && currentSync->flag == 0 && getCounter(currentSync) == 4) {
                Sync *newSync = initSync(currentSync->parent, 1,
                                           currentSync->x_i, currentSync->x_j,
                                           currentSync->y_i, currentSync->y_j,
                                           currentSync->z_i, currentSync->z_j,
                                           currentSync->n);

                MMSecondHalf(newSync->x_i, newSync->x_j,
                             newSync->y_i, newSync->y_j,
                             newSync->z_i, newSync->z_j,
                             newSync->n, newSync, id);
                    break;
            } else if(currentSync && currentSync->flag == 1 && getCounter(currentSync) == 4) {
                currentSync = currentSync->parent;
                if(currentSync != NULL)
                    incrementCounter(currentSync);
                else {
                    workDone = 1;
                }
            }
            else {
                break;
            }
        }
    }
}

void MMFirstHalf(int x_i, int x_j, int y_i, int y_j, int z_i, int z_j, int n, Sync *sync, int id)
{
    if (n == grain_size){
        MMBase(x_i, x_j, y_i, y_j, z_i, z_j, sync, id);
        return;
    }

    Sync *newSync = initSync(sync, 0, x_i, x_j, y_i, y_j, z_i, z_j, n);
    pushBackTask(initTask(x_i, x_j, y_i, y_j, z_i, z_j, n/2, newSync, 0), randomNumber(id));
    pushBackTask(initTask(x_i, x_j, y_i, y_j + n/2, z_i, z_j + n/2, n/2, newSync, 0), randomNumber(id));
    pushBackTask(initTask(x_i + n/2, x_j, y_i, y_j, z_i + n/2, z_j, n/2, newSync, 0), randomNumber(id));
    pushBackTask(initTask(x_i + n/2, x_j, y_i, y_j + n/2, z_i + n/2, z_j + n/2, n/2, newSync, 0), randomNumber(id));
}

void MMSecondHalf(int x_i, int x_j, int y_i, int y_j, int z_i, int z_j, int n, Sync *sync, int id)
{
    pushBackTask(initTask(x_i, x_j + n/2, y_i + n/2, y_j, z_i, z_j, n/2, sync, 1), randomNumber(id));
    pushBackTask(initTask(x_i, x_j + n/2, y_i + n/2, y_j + n/2, z_i, z_j + n/2, n/2, sync, 1), randomNumber(id));
    pushBackTask(initTask(x_i + n/2, x_j + n/2, y_i + n/2, y_j, z_i + n/2, z_j, n/2, sync, 1), randomNumber(id));
    pushBackTask(initTask(x_i + n/2, x_j + n/2, y_i + n/2, y_j + n/2, z_i + n/2, z_j + n/2, n/2, sync, 1), randomNumber(id));
}

Sync* initSync(Sync *parent, int flag, int x_i, int x_j, int y_i, int y_j, int z_i, int z_j, int n)
{
    Sync *newSync = new Sync;

    newSync->flag = flag;
    newSync->count = 0;
    newSync->x_i = x_i;
    newSync->x_j = x_j;
    newSync->y_i = y_i;
    newSync->y_j = y_j;
    newSync->z_i = z_i;
    newSync->z_j = z_j;
    newSync->n = n;
    newSync->parent = parent;

    return newSync;
}

Task *initTask(int x_i, int x_j, int y_i, int y_j, int z_i, int z_j, int n, Sync *sync, int flag)
{
    Task *newTask = new Task;

    newTask->functionPara.n = n;
    newTask->functionPara.x_i = x_i;
    newTask->functionPara.x_j = x_j;
    newTask->functionPara.y_i = y_i;
    newTask->functionPara.y_j = y_j;
    newTask->functionPara.z_i = z_i;
    newTask->functionPara.z_j = z_j;
    newTask->functionPara.sync = sync;

    return newTask;
}

void pushBackTask(Task* toPush, int threadID)
{
    pthread_mutex_lock(&dequeLock[threadID]);
    myQueue[threadID].push_back(toPush);
    pthread_mutex_lock(&workLock);
    workLoad[threadID] = workLoad[threadID] + pow(toPush->functionPara.n, 3);
    pthread_mutex_unlock(&workLock);
    pthread_mutex_unlock(&dequeLock[threadID]);
}

void popFrontTask(int threadID)
{
  pthread_mutex_lock(&dequeLock[threadID]);
  myQueue[threadID].pop_front();
  pthread_mutex_unlock(&dequeLock[threadID]);
}

void pushFrontTask(Task* toPush, int threadID)
{
  pthread_mutex_lock(&dequeLock[threadID]);
  myQueue[threadID].push_front(toPush);
  pthread_mutex_unlock(&dequeLock[threadID]);
}

Task *getFrontTask(int threadID)
{
  Task *newTask;
  pthread_mutex_lock(&dequeLock[threadID]);
  newTask = myQueue[threadID].front();
  pthread_mutex_unlock(&dequeLock[threadID]);

  return newTask;
}

Task *getBackTask(int threadID)
{
    pthread_mutex_lock(&dequeLock[threadID]);
    Task *newTask;

    if(myQueue[threadID].empty()) {
        pthread_mutex_unlock(&dequeLock[threadID]);
        return NULL;
    }
    newTask = myQueue[threadID].back();
    myQueue[threadID].pop_back();
    pthread_mutex_lock(&workLock);
    workLoad[threadID] = workLoad[threadID] - pow(newTask->functionPara.n, 3);
    pthread_mutex_unlock(&workLock);
    pthread_mutex_unlock(&dequeLock[threadID]);

    return newTask;
}

void runThread(int threadID)
{
    int rand_num = threadID;

    while(!workDone){
        if(myQueue[threadID].size()!=0){
                rand_num = threadID;
	}
        else{
            while(myQueue[rand_num].size() == 0) {
                rand_num = randomNumber(rand_num);
		break;
            }
	}

        Task *newTask = getBackTask(rand_num);
        if (newTask) {
            MMFirstHalf(newTask->functionPara.x_i, newTask->functionPara.x_j,
                         newTask->functionPara.y_i, newTask->functionPara.y_j,
                         newTask->functionPara.z_i, newTask->functionPara.z_j,
                         newTask->functionPara.n, newTask->functionPara.sync, threadID);
        }
    }
}


int main(int argc, char *argv[])
{
    int k = 12;
    int i, j, n = pow(2, k);

    for(i = 0; i < num_threads; i++)
        pthread_mutex_init(&dequeLock[i], NULL);

    pthread_mutex_init(&workLock, NULL);
    pthread_mutex_init(&syncLock, NULL);


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
	cout << "generated matrices\n";
	auto start = std::chrono::high_resolution_clock::now();
	pushBackTask(initTask(0, 0, 0, 0, 0, 0, n, NULL, 0), 0);

	for(i = 0; i < num_threads; ++i)
		cilk_spawn runThread(i);
	runThread(0);
	cilk_sync;

	auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish - start;
	std::cout << "Elapsed time: " << elapsed.count() << " s\n";
	
	float totalFloatOP = 2 * pow(n, 3);
	float runTime = elapsed.count();
	float FLOPS = totalFloatOP / runTime;
	float GFLOPS = FLOPS / pow(10, 9);

	cout << "Rate of Execution (GFLOPS): " << GFLOPS <<endl;	

	return 0;
}
