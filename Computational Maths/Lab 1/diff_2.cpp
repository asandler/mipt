#include <iostream>
#include <math.h>

using namespace std;

double abs(double x) {
    return (x < 0) ? -x : x;
}

double err(double h) {
    double t = (exp((1 + h) * 2) - exp((1 - h) * 2)) / (2 * h);
    return abs(exp(2) * 2 - t);
}

int main() {
    for (double h = 10e-9; h < 10e-5; h += 10e-9) {
        cout << err(h) << endl;
    }
    return 0;
}
