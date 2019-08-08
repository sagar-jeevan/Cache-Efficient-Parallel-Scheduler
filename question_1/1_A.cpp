#include<iostream>
#include <vector>
#include <time.h>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <random>

using namespace std;

std::vector< std::vector<int> > X;
std::vector< std::vector<int> > Y;
std::vector< std::vector<int> > Z;

void iter_mm_ijk(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n);
void iter_mm_ikj(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n);
void iter_mm_jik(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n);
void iter_mm_jki(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n);
void iter_mm_kij(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n);
void iter_mm_kji(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n);

int main (){
    int k = 12;
    int i, j;
    int  n = pow(2, k);
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
    iter_mm_ijk(Z, X, Y, n);
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << elapsed.count() << " s\n";

    for (auto &i : Z)
        std::fill(i.begin(), i.end(), 0);
    start = std::chrono::high_resolution_clock::now();
    iter_mm_ikj(Z, X, Y, n);
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    std::cout  << elapsed.count() << " s\n";

    for (auto &i : Z)
        std::fill(i.begin(), i.end(), 0);
    start = std::chrono::high_resolution_clock::now();
    iter_mm_jik(Z, X, Y, n);
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    std::cout << elapsed.count() << " s\n";

    for (auto &i : Z)
        std::fill(i.begin(), i.end(), 0);
    start = std::chrono::high_resolution_clock::now();
    iter_mm_jki(Z, X, Y, n);
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    std::cout << elapsed.count() << " s\n";

    for (auto &i : Z)
        std::fill(i.begin(), i.end(), 0);
    start = std::chrono::high_resolution_clock::now();
    iter_mm_kij(Z, X, Y, n);
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    std::cout<< elapsed.count() << " s\n";

    for (auto &i : Z)
        std::fill(i.begin(), i.end(), 0);
    start = std::chrono::high_resolution_clock::now();
    iter_mm_kji(Z, X, Y, n);
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    std::cout  << elapsed.count() << " s\n";

    return 0;
}

void iter_mm_ijk(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n){
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            for(int k=0;k<n;k++){
                Z[i][j] = Z[i][j] + X[i][k] * Y[k][j];
            }
        }
    }
}
void iter_mm_ikj(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n){
    for(int i=0;i<n;i++){
        for(int k=0;k<n;k++){
            for(int j=0;j<n;j++){
                Z[i][j] = Z[i][j] + X[i][k] * Y[k][j];
            }
        }
    }
}

void iter_mm_jik(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n){
    for(int j=0;j<n;j++){
        for(int i=0;i<n;i++){
            for(int k=0;k<n;k++){
                Z[i][j] = Z[i][j] + X[i][k] * Y[k][j];
            }
        }
    }
}
void iter_mm_jki(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n){
    for(int j=0;j<n;j++){
        for(int k=0;k<n;k++){
            for(int i=0;i<n;i++){
                Z[i][j] = Z[i][j] + X[i][k] * Y[k][j];
            }
        }
    }
}

void iter_mm_kij(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n){
    for(int k=0;k<n;k++){
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                Z[i][j] = Z[i][j] + X[i][k] * Y[k][j];
            }
        }
    }
}

void iter_mm_kji(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n){
    for(int k=0;k<n;k++){
        for(int j=0;j<n;j++){
            for(int i=0;i<n;i++){
                Z[i][j] = Z[i][j] + X[i][k] * Y[k][j];
            }
        }
    }
}
