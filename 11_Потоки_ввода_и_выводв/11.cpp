#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

int main() {
    ifstream fin("coordinates.txt", ios::in);
    if (fin.is_open()) {
        double x, y;
        bool hasCoor = false;
        string s;
        cout.setf(ios::fixed);
        cout.precision(6);
        while (getline(fin, s)) {
            stringstream sin(s);
            sin.ignore(1);
            sin >> x;
            sin.ignore(2);
            sin >> y;
            if (x >= 50 && x < 80 && y >= 20 && y < 45) {
                cout << '(' << x << ", " << y << ')' << endl;
                hasCoor = true;
            }
        }
        if (!hasCoor) {
            cout << "No coordinates found in a square with latitude from 50 to 80 longitude from 20 to 45" << endl;
        }
    }
    else {
        cout << "File coordinates.txt not found" << endl;
    }
    return 0;
}