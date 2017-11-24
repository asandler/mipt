#include <iostream>
#include <math.h>

using namespace std;

const long double eps = 1e-6;
const long double pi = acos((long double)-1);

long double f(long double x, long double a) {
    return exp(-(x * x) / a);
}

long double integral(long double x, long double h, long double sigma) {
    long double ans, cur, m = 2 * sigma * sigma;
    ans = f(x, m) / 2;
    int i = 1;
    do {
        cur = f(x - i * h, m);
        ans += cur;
        i++;
    } while (cur > eps || x - i * h > 0);
    ans += f(x - i * h, m) / 2;
    return ans * h / (sqrt(2 * pi) * sigma);
}

int main() {
    long double x, sigma, h;
    cout << "x = ";
    cin >> x;
    cout << "sigma = ";
    cin >> sigma;
    cout.precision(10);

    /*integral*/
    h = sqrt(12 * sigma) * sigma * sqrt(eps) * exp(3 / 4);
    cout << integral(x, h, sigma) << endl;

    /*linear interpolation*/
    h = 2 * sqrt(eps * sigma) * exp(1 / 4);
    cout << "linear interpolation, h = " << h << endl;
    long double prevx = 0, prevy = 0, xint = 0;
    for (x = -1; x < 1; x += h) {
        long double y = integral(x, h, sigma);
        if (y - 0.3 < 0.01 && y - 0.3 > -0.01) {
            cout << "x = " << x << "\tf(x) = " << y << endl;
            if (prevy < 0.3 && y > 0.3) {
                xint = prevx + (0.3 - prevy) * (x - prevx) / (y - prevy);
            }
            prevx = x;
            prevy = y;
        }
    }
    cout << "f^(-1)(0.3) = " << xint << endl;

    /*square interpolation*/
    h = exp(0.5) * pow(3.75f * eps * sigma * sigma, 1.0 / 3);
    cout << "square interpolation, h = " << h << endl;
    long double x0 = 0, x1 = 0, x2 = 0, y0 = 0, y1 = 0, y2 = 0;
    bool flag = true;
    for (x = -1; x < 1; x += h) {
        long double y = integral(x, h, sigma);
        if (y - 0.3 < 0.05 && y - 0.3 > -0.05) {
            cout << "x = " << x << "\tf(x) = " << y << endl;
            if (y > 0.3 && flag) {
                long double c1 = y0 * (x2 - x1);
                long double c2 = y1 * (x2 - x0);
                long double c3 = y2 * (x1 - x0);
                long double k1 = c1 - c2 + c3;
                long double k2 = c1 * (-x1 - x2) - c2 * (-x0 - x2) + c3 * (-x0 - x1);
                long double k3 = c1 * x1 * x2 - c2 * x0 * x2 + c3 * x0 * x1;
                k3 -= 0.3 * (x2 - x1) * (x1 - x0) * (x2 - x0);
                long double d = k2 * k2 - 4 * k1 * k3;
                xint = (-k2 + sqrt(d)) / (2 * k1);
                flag = false;
            }
            x0 = x1;
            x1 = x2;
            x2 = x;
            y0 = y1;
            y1 = y2;
            y2 = y;
        }
    }
    cout << "f^(-1)(0.3) = " << xint << endl;
    return 0;
}
