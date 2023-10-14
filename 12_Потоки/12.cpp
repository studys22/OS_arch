#include <iostream>
#include <iomanip>
#include <vector>
#include <pthread.h>
#include <unistd.h>
using namespace std;

#define Rows 5
#define Columns 6
#define A_columns_B_rows 3
#define maxValue 100

#define ERROR_CREATE_THREAD -11
#define ERROR_JOIN_THREAD   -12
#define BAD_MESSAGE         -13
#define SUCCESS               0

#define THREADS  8

typedef vector<vector<int> > matr;

void printMatrix(matr& a, int w) {
    cout.setf(ios::right);
    for (int i = 0; i < a.size(); i++) {
        for (int j = 0; j < a[i].size(); j++) {
            cout << setw(w) << a[i][j];
        }
        cout << endl;
    }
    cout << endl;
    cout.unsetf(ios::right);
}

void fillMatrix(matr& a) {
    int t;
    for (int i = 0; i < a.size(); i++) {
        for (int j = 0; j < a[i].size(); j++) {
            t = rand();
            a[i][j] = t % maxValue;
        }
    }
}

matr createMatrix(int n, int m, bool fill) {
    matr a(n, vector<int>(m));
    if (fill)
        fillMatrix(a);
    return a;
}

typedef struct structData {
    matr a, b, c;
    structData() : a(createMatrix(Columns, A_columns_B_rows, true)), b(createMatrix(A_columns_B_rows, Rows, true)), c(createMatrix(Rows, Columns, false)) {
    }
} structData_t;

void* proc(void* args) {

    pair<structData_t*, int* >* data = (pair<structData_t*, int* >*)args;
    int res = 0;
    int i = data->second[0], j = data->second[1];

    for (int k = 0; k < A_columns_B_rows; k++) {
        res += data->first->a[j][k] * data->first->b[k][i];
    }
    data->first->c[i][j] = res;

    return SUCCESS;
}

void closeTreads(vector<pthread_t>& pt, int beg, int end) {
    long status, status_addr;
    for (int t = beg; t < end; t++) {
        status = pthread_join(pt[t], (void**)&status_addr);
        if (status != SUCCESS) {
            printf("main error: can't join thread, status = %ld\n", status);
            exit(ERROR_JOIN_THREAD);
        }
    }
}

int main() {
    srand(time(0));
    structData_t data;
    printMatrix(data.a, 4);
    printMatrix(data.b, 4);

    vector<pthread_t> pt(THREADS);
    int t = 0;
    long status, status_addr;
    pair<structData_t*, int* > params[THREADS];
    for (int i = 0; i < THREADS; i++) {
        params[i].first = &data;
    }

    for (int i = 0; i < Rows; i++) {
        for (int j = 0; j < Columns; j++) {
            if (t == THREADS) {
                closeTreads(pt, 0, THREADS);
                t = 0;
            }

            params[t].second = new int[2] {i, j};

            status = pthread_create(&pt[t], NULL, proc, (void*)&params[t]);
            t++;
            if (status != 0) {
                printf("main error: can't create thread, status = %ld\n", status);
                exit(ERROR_CREATE_THREAD);
            }
        }
    }
    closeTreads(pt, 0, t);

    printMatrix(data.c, 6);

    return 0;
}