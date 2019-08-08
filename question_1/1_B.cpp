#include<iostream>
#include <vector>
#include <time.h>
#include <fstream>
#include <sstream>
#include <string>
#include <papi.h>

using namespace std;

void iter_mm_ijk(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n);
void iter_mm_ikj(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n);
void iter_mm_jik(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n);
void iter_mm_jki(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n);
void iter_mm_kij(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n);
void iter_mm_kji(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n);

long long counters[3];
int PAPI_events[] = {
    PAPI_L1_TCM,
    PAPI_L2_TCM,
    PAPI_L3_TCM
};

int main(int arc, char *argv[])
{
    int i, j, k = atoi(argv[1]);
    int  n = pow(2, k);

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

    clock_t tStart = clock();
    iter_mm_ijk(Z, X, Y, n);
    std::cout << "The time taken by ijk in secs is :" << ((clock()- tStart) / CLOCKS_PER_SEC);
    std::cout << "Matrix Z contents are: \n";

    for (auto &i : Z)
        std::fill(i.begin(), i.end(), 0);

    tStart = clock();
    iter_mm_ikj(Z, X, Y, n);
    std::cout << "The time taken by ikj in secs is :" << ((clock()- tStart) / CLOCKS_PER_SEC);
    std::cout << "Matrix Z contents are: \n";

    for (auto &i : Z)
        std::fill(i.begin(), i.end(), 0);
    tStart = clock();
    iter_mm_jik(Z, X, Y, n);

    std::cout << "The time taken by jik in secs is:" << ((clock()- tStart)/CLOCKS_PER_SEC);
    std::cout << "Matrix Z contents are: \n";

    for (auto &i : Z)
        std::fill(i.begin(), i.end(), 0);
    tStart = clock();
    iter_mm_jki(Z, X, Y, n);

    std::cout << "The time taken by jki in secs is :" << ((clock()- tStart)/CLOCKS_PER_SEC);
    std::cout << "Matrix Z contents are: \n";

    for (auto &i : Z)
        std::fill(i.begin(), i.end(), 0);
    tStart = clock();
    iter_mm_kij(Z, X, Y, n);

    std::cout << "The time taken by kij in secs is :" << ((clock()- tStart)/CLOCKS_PER_SEC);
    std::cout << "Matrix Z contents are: \n";

    for (auto &i : Z)
        std::fill(i.begin(), i.end(), 0);
    tStart = clock();
    iter_mm_kji(Z, X, Y, n);

    std::cout << "The time taken by kji in secs is :" << ((clock()- tStart)/CLOCKS_PER_SEC);
    std::cout << "Matrix Z contents are: \n";

    return 0;
}

void iter_mm_ijk(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n){
    int count = PAPI_start_counters( PAPI_events, 3 );
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            for(int k=0;k<n;k++){
                Z[i][j] = Z[i][j] + X[i][k] * Y[k][j];
            }
        }
    }

    PAPI_stop_counters( counters, 3 );
    PAPI_read_counters(counters,3);
    for(int i =0;i<3;i++)
        cout<<"counters[i]"<<i<<"is"<<counters[i]<<endl;

}
void iter_mm_ikj(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n){
    int count = PAPI_start_counters( PAPI_events, 3 );
    for(int i=0;i<n;i++)
        for(int k=0;k<n;k++)
            for(int j=0;j<n;j++)
                    Z[i][j] = Z[i][j] + X[i][k] * Y[k][j];
                }
            }
        }
    PAPI_stop_counters( counters, 3 );
    PAPI_read_counters(counters,3);
    for(int i =0;i<3;i++)
        cout<<"counters[i]"<<i<<"is"<<counters[i]<<endl;
}

void iter_mm_jik(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n){
    int count = PAPI_start_counters( PAPI_events, 3 );
        for(int j=0;j<n;j++){
            for(int i=0;i<n;i++){
                for(int k=0;k<n;k++){
                    Z[i][j] = Z[i][j] + X[i][k] * Y[k][j];
                }
            }
        }
    PAPI_stop_counters( counters, 3 );
    PAPI_read_counters(counters,3);
    for(int i =0;i<3;i++)
        cout<<"counters[i]"<<i<<"is"<<counters[i]<<endl;
}
void iter_mm_jki(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n){
    int count = PAPI_start_counters( PAPI_events, 3 );
        for(int j=0;j<n;j++){
            for(int k=0;k<n;k++){
                for(int i=0;i<n;i++){
                    Z[i][j] = Z[i][j] + X[i][k] * Y[k][j];
                }
            }
        }
    PAPI_stop_counters( counters, 3 );
    PAPI_read_counters(counters,3);
    for(int i =0;i<3;i++)
        cout<<"counters[i]"<<i<<"is"<<counters[i]<<endl;
}

void iter_mm_kij(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n){
    int count = PAPI_start_counters( PAPI_events, 3 );
        for(int k=0;k<n;k++){
            for(int i=0;i<n;i++){
                for(int j=0;j<n;j++){
                    Z[i][j] = Z[i][j] + X[i][k] * Y[k][j];
                }
            }
        }
    PAPI_stop_counters( counters, 3 );
    PAPI_read_counters(counters,3);
    for(int i =0;i<3;i++)
        cout<<"counters[i]"<<i<<"is"<<counters[i]<<endl;
}

void iter_mm_kji(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n){
    int count = PAPI_start_counters( PAPI_events, 3 );
        for(int k=0;k<n;k++){
            for(int j=0;j<n;j++){
                for(int i=0;i<n;i++){
                    Z[i][j] = Z[i][j] + X[i][k] * Y[k][j];
                }
            }
        }
    PAPI_stop_counters( counters, 3 );
    PAPI_read_counters(counters,3);
    for(int i =0;i<3;i++)
        cout<<"counters[i]"<<i<<"is"<<counters[i]<<endl;
}
