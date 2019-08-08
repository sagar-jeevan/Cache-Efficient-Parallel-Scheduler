#include<iostream>
#include <vector>
using namespace std;
#include <time.h>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <random>
#include <stdio.h>
#include <stdlib.h>

void compare_file(vector< std::vector<int> > &Z, const char string[]);


void iter_mm_ikj1(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n);
void iter_mm_kji1(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n);

void iter_mm_ikj2(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n);
void iter_mm_kji2(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n);

void iter_mm_ikj3(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n);
void iter_mm_kji3(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n);

void iter_mm_ikj4(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n);
void iter_mm_kji4(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n);

void iter_mm_ikj5(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n);
void iter_mm_kji5(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n);

void iter_mm_ikj6(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n);
void iter_mm_kji6(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n);

int main (){
    //int n;
    std::vector< std::vector<int> > X;
    std::vector< std::vector<int> > Y;
    std::vector< std::vector<int> > Z;
    int k = 11;
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


    for (auto &i : Z)
        std::fill(i.begin(), i.end(), 0);

    auto start = std::chrono::high_resolution_clock::now();
    iter_mm_ikj1(Z, X, Y, n);
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << elapsed.count() << " s\n";
    //compare_file(Z, "ikj1");

    /*for (auto &i : Z)
        std::fill(i.begin(), i.end(), 0);

    start = std::chrono::high_resolution_clock::now();
    iter_mm_ikj2(Z, X, Y, n);
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    std::cout << elapsed.count() << " s\n";
    //compare_file(Z, "ikj2");*/

    for (auto &i : Z)
        std::fill(i.begin(), i.end(), 0);

    start = std::chrono::high_resolution_clock::now();
    iter_mm_ikj3(Z, X, Y, n);
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    std::cout << elapsed.count() << " s\n";
    compare_file(Z, "ikj3");

    for (auto &i : Z)
        std::fill(i.begin(), i.end(), 0);

    start = std::chrono::high_resolution_clock::now();
    iter_mm_ikj4(Z, X, Y, n);
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    std::cout << elapsed.count() << " s\n";
    compare_file(Z, "ikj4");

    for (auto &i : Z)
        std::fill(i.begin(), i.end(), 0);

    start = std::chrono::high_resolution_clock::now();
    iter_mm_ikj5(Z, X, Y, n);
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    std::cout << elapsed.count() << " s\n";
    compare_file(Z, "ikj5");


    for (auto &i : Z)
        std::fill(i.begin(), i.end(), 0);
    start = std::chrono::high_resolution_clock::now();
    iter_mm_kji2(Z, X, Y, n);
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    std::cout << elapsed.count() << " s\n";
    compare_file(Z, "kji2");

    for (auto &i : Z)
        std::fill(i.begin(), i.end(), 0);
    start = std::chrono::high_resolution_clock::now();
    iter_mm_kji3(Z, X, Y, n);
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    std::cout << elapsed.count() << " s\n";
    compare_file(Z, "kji3");


    for (auto &i : Z)
        std::fill(i.begin(), i.end(), 0);
    start = std::chrono::high_resolution_clock::now();
    iter_mm_kji6(Z, X, Y, n);
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    std::cout <<  elapsed.count() << " s\n";
    compare_file(Z, "kji6");
    return 0;
}
//template<int n, i
void iter_mm_ikj1(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n){
    #pragma omp parallel for
    for(int i=0;i<n;i++){
        for(int k=0;k<n;k++){
            for(int j=0;j<n;j++){
                Z[i][j] = Z[i][j] + X[i][k] * Y[k][j];
            }
        }
    }
}



void iter_mm_kji1(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n){
    #pragma omp parallel for
    for(int k=0;k<n;k++){
        for(int j=0;j<n;j++){
            for(int i=0;i<n;i++){
                Z[i][j] = Z[i][j] + X[i][k] * Y[k][j];
            }
        }
    }
}

void iter_mm_ikj2(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n){
    for(int i=0;i<n;i++){
        #pragma omp parallel for
        for(int k=0;k<n;k++){
            for(int j=0;j<n;j++){
                Z[i][j] = Z[i][j] + X[i][k] * Y[k][j];
            }
        }
    }
}




void iter_mm_kji2(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n){
    for(int k=0;k<n;k++){
        #pragma omp parallel for
        for(int j=0;j<n;j++){
            for(int i=0;i<n;i++){
                Z[i][j] = Z[i][j] + X[i][k] * Y[k][j];
            }
        }
    }
}

void iter_mm_ikj3(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n){
    for(int i=0;i<n;i++){
        for(int k=0;k<n;k++){
            #pragma omp parallel for
            for(int j=0;j<n;j++){
                Z[i][j] = Z[i][j] + X[i][k] * Y[k][j];
            }
        }
    }
}



void iter_mm_kji3(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n){
    for(int k=0;k<n;k++){
        for(int j=0;j<n;j++){
            #pragma omp parallel for
            for(int i=0;i<n;i++){
                Z[i][j] = Z[i][j] + X[i][k] * Y[k][j];
            }
        }
    }
}

void iter_mm_ikj4(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n){
    #pragma omp parallel for
    for(int i=0;i<n;i++){
        #pragma omp parallel for
        for(int k=0;k<n;k++){
            for(int j=0;j<n;j++){
                Z[i][j] = Z[i][j] + X[i][k] * Y[k][j];
            }
        }
    }
}


void iter_mm_kji4(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n){
    #pragma omp parallel for
    for(int k=0;k<n;k++){
        #pragma omp parallel for
        for(int j=0;j<n;j++){
            for(int i=0;i<n;i++){
                Z[i][j] = Z[i][j] + X[i][k] * Y[k][j];
            }
        }
    }
}

void iter_mm_ikj5(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n){
    #pragma omp parallel for
    for(int i=0;i<n;i++){
        for(int k=0;k<n;k++){
            #pragma omp parallel for
            for(int j=0;j<n;j++){
                Z[i][j] = Z[i][j] + X[i][k] * Y[k][j];
            }
        }
    }
}

void iter_mm_kji5(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n){
    #pragma omp parallel for
    for(int k=0;k<n;k++){
        for(int j=0;j<n;j++){
            #pragma omp parallel for
            for(int i=0;i<n;i++){
                Z[i][j] = Z[i][j] + X[i][k] * Y[k][j];
            }
        }
    }
}

void iter_mm_ikj6(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n){
    for(int i=0;i<n;i++){
        #pragma omp parallel for
        for(int k=0;k<n;k++){
            #pragma omp parallel for
            for(int j=0;j<n;j++){
                Z[i][j] = Z[i][j] + X[i][k] * Y[k][j];
            }
        }
    }
}

void iter_mm_kji6(vector< std::vector<int> > &Z, vector< std::vector<int> > X, vector< std::vector<int> > Y,int n){
    for(int k=0;k<n;k++){
        #pragma omp parallel for
        for(int j=0;j<n;j++){
            #pragma omp parallel for
            for(int i=0;i<n;i++){
                Z[i][j] = Z[i][j] + X[i][k] * Y[k][j];
            }
        }
    }
}

void compare_file(vector< std::vector<int> > &Z, const char string[]){
    ofstream myfile;
      myfile.open ("out_2_8.txt");
      for(int i=0;i<Z.size();++i) {
        for(int j=0;j<Z[0].size();++j){
            myfile << Z[i][j] << " ";
        }
        myfile << "\n";
        }
      myfile.close();
      int ret = system("cmp out_2_8.txt Z_2_8.txt");
    if(ret == 0)
        cout<<string <<" correct"<<endl;
    else
        cout<<string <<"wrong"<< endl;
}
