#include <iostream>
#include <math.h>

using namespace std;

const double h = 1e-2;

double abs(double x) {
    return (x < 0) ? -x : x;
}

double f1(double x) {
    return exp(x) / (x * x + 1);
}
double f1_(double x) {
    return exp(x) * pow((x - 1), 2) / pow((x * x + 1), 2);
}

double f2(double x) {
    return 4 * pow(x, 3) - x * x;
}
double f2_(double x) {
    return 12 * x * x - 2 * x;
}

double f3(double x) {
    return 1 / pow(x + 4, 3);
}
double f3_(double x) {
    return -3 / pow(x + 4, 4);
}

double diff1(double (*f)(double x), double (*f_)(double x)) {
    double max_delta = 0;
    for (double p = -1; p < 1; p += h) {
        double t = (f(p + h) - f(p - h)) / (2 * h);
        max_delta = max(max_delta, abs(f_(p) - t));
    }
    return max_delta;
}

double diff2(double (*f)(double x), double (*f_)(double x)) {
    double max_delta = 0;
    for (double p = -1; p < 1; p += h) {
        double t = (f(p - 2 * h) - 4 * f(p - h) + 3 * f(p)) / (2 * h);
        max_delta = max(max_delta, abs(f_(p) - t));
    }
    return max_delta;
}

double diff4(double (*f)(double x), double (*f_)(double x)) {
    double max_delta = 0;
    for (double p = -1; p < 1; p += h) {
        double t = (f(p - 2 * h) - 8 * f(p - h) + 8 * f(p + h) - f(p + 2 * h)) / (12 * h);
        max_delta = max(max_delta, abs(f_(p) - t));
    }
    return max_delta;
}

int main() {
    cout << diff2(f1, f1_) << endl;
    return 0;
}
