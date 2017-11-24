#include <iostream>
#include <math.h>

using namespace std;

const double h = 1e-2;

double f(double x, double y) {
    return 4 * x * x - y * y;
}

int main() {
    double y = 1, x = 0;
    cout << x << ' ' << y << endl;
    for (int i = 0; i < 100; i++) {
        double k1 = h * f(x, y);
        double k2 = h * f(x + 1 / 3 * h, y + 1 / 3 * k1);
        double k3 = h * f(x + 2 / 3 * h, y - 1 / 3 * k1 + 1 * k2);
        double k4 = h * f(x + 1 * h, y + 1 * k1 - 1 * k2 + 1 * k3);
        y = y + k1 / 8 + 3 * k2 / 8 + 3 * k3 / 8 + k4 / 8;
        x += h;
        cout << x << ' ' << y << endl;
    }
    return 0;
}
