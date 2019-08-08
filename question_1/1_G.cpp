#include<iostream>
#include <vector>
using namespace std;
#include <time.h>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <chrono>

std::vector< std::vector<int> > X;
std::vector< std::vector<int> > Y;
std::vector< std::vector<int> > Z;

void iter_mm_ikj(int x_i, int x_j, int y_i, int y_j, int z_i, int z_j, int n);
void iter_mm_ikj(int x_i, int x_j, int y_i, int y_j, int z_i, int z_j, int n){
    for(int i=0;i<n;i++){
        for(int k=0;k<n;k++){
            for(int j=0;j<n;j++){
                Z[i][j] = Z[i][j] + X[i][k] * Y[k][j];
            }
        }
    }
}
void matrixmul(int x_i, int x_j, int y_i, int y_j, int z_i, int z_j, int n){
    if (n == 64){
        iter_mm_ikj(x_i, x_j, y_i, y_j, z_i, z_j, n);
    }
    else{
            #pragma omp parallel
            {
                matrixmul(x_i, x_j, y_i, y_j, z_i, z_j, n/2);
                matrixmul(x_i, x_j, y_i, y_j + n/2, z_i, z_j + n/2, n/2);
                matrixmul(x_i + n/2, x_j, y_i, y_j, z_i + n/2, z_j, n/2);
                matrixmul(x_i + n/2, x_j, y_i, y_j + n/2, z_i + n/2, z_j + n/2, n/2);
            }
            #pragma omp barrier
            #pragma omp parallel
            {
                matrixmul(x_i, x_j + n/2, y_i + n/2, y_j, z_i, z_j, n/2);
                matrixmul(x_i, x_j + n/2, y_i + n/2, y_j + n/2, z_i, z_j + n/2, n/2);
                matrixmul(x_i + n/2, x_j + n/2, y_i + n/2, y_j, z_i + n/2, z_j, n/2);
                matrixmul(x_i + n/2, x_j + n/2, y_i + n/2, y_j + n/2, z_i + n/2, z_j + n/2, n/2);
            }
    }
}

int main(int arc, char *argv[])
{
    int i, j, k = atoi(argv[1]);
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
    auto start = std::chrono::high_resolution_clock::now();
    matrixmul(0, 0, 0, 0, 0, 0, n);
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Elapsed time: " << elapsed.count() << " s\n";

}
