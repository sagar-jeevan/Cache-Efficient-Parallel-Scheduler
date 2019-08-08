#include <iostream>
#include <vector>
#include <time.h>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <papi.h>

using namespace std;

void compare_file(vector< std::vector<int> > &Z, const char string[]);
void iter_mm_ikj4(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n);

int main(int argc, char *argv[])
{
    int i, j, n = pow(2, k), k = atoi(argv[1]);
    long long counters[3];
    int PAPI_events[] = {
        PAPI_L1_TCM,
        PAPI_L2_TCM,
        PAPI_L3_TCM,
    };

    PAPI_library_init(PAPI_VER_CURRENT);

    std::vector< std::vector<int> > X;
    std::vector< std::vector<int> > Y;
    std::vector< std::vector<int> > Z;

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

    i = PAPI_start_counters(PAPI_events, 3 );
    auto start = std::chrono::high_resolution_clock::now();

    iter_mm_ikj4(Z, X, Y, n);

    auto finish = std::chrono::high_resolution_clock::now();
    auto elapsed = finish - start;
    std::cout << "Elapsed time: " << elapsed.count() << " s\n";

    PAPI_read_counters( counters, 3 );

    cout << counters[0] << " " << counters[1] << " " << counters[2] << endl;
    return 0;
}

void iter_mm_ikj4(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n){
    #pragma omp parallel for
    for(int i = 0; i < n; i++){
        #pragma omp parallel for
        for(int k = 0; k < n; k++){
            for(int j = 0; j<n; j++){
                Z[i][j] = Z[i][j] + X[i][k] * Y[k][j];
            }
        }
    }
}
