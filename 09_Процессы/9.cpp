#include <unistd.h>
#include <iostream>
using namespace std;

#define Rows 5
#define Columns 6
#define A_columns_B_rows 3

void print_arr(int** a, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout << a[i][j] << ' ';
        }
        cout << endl;
    }
    cout << endl;
}

int main()
{
    int** a = new int* [Columns];
    int** b = new int* [A_columns_B_rows];
    srand(time(0));

    for (int i = 0; i < Columns; i++) {
        a[i] = new int[A_columns_B_rows];
        for (int j = 0; j < A_columns_B_rows; j++) {
            int t = rand();
            a[i][j] = t % 100;
        }
    }
    print_arr(a, Columns, A_columns_B_rows);
    for (int i = 0; i < A_columns_B_rows; i++) {
        b[i] = new int[Rows];
        for (int j = 0; j < Rows; j++) {
            b[i][j] = rand() % 100;
        }
    }
    print_arr(b, A_columns_B_rows, Rows);

    int pipes[Rows][Columns][2];

    for (int i = 0; i < Rows; i++) {
        for (int j = 0; j < Columns; j++) {
            if (pipe(pipes[i][j]) == -1) {
                cerr << "Error: pipe creation failed" << endl;
                return 1;
            }
            int pid = fork();
            if (pid < 0) {
                cerr << "Error: fork failed" << endl;
            }
            else if (pid > 0) {
                close(pipes[i][j][1]);
            }
            else {
                close(pipes[i][j][0]);
                int res = 0;
                for (int k = 0; k < A_columns_B_rows; k++) {
                    res += a[j][k] * b[k][i];
                }
                int sz = sizeof(res);
                char buf[sz];
                for (int k = 0; k < sz; k++) {
                    buf[k] = res % 256;
                    res /= 256;
                }

                if (write(pipes[i][j][1], buf, sz) == -1) {
                    cerr << "Error: write to pipe failed" << endl;
                    return 1;
                }
                close(pipes[i][j][1]);
                return 0;
            }
        }
    }
    int** c = new int* [Rows];
    for (int i = 0; i < Rows; i++) {
        c[i] = new int[Columns];
        for (int j = 0; j < Columns; j++) {
            int res;
            int sz = sizeof(res);
            char buf[sz];
            if (read(pipes[i][j][0], buf, sz) == -1) {
                cerr << "Error: read from pipe failed" << endl;
                return 1;
            }
            for (int k = sz - 1; k >= 0; k--) {
                res = res * 256 + (int)buf[k];
            }
            c[i][j] = res;
        }
    }
    print_arr(c, Rows, Columns);

    return 0;
}