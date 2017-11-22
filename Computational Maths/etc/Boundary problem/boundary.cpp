#include <iostream>
#include <math.h>

using namespace std;

double u[1000][1000];
double h = 1e-2, tau = 1e-2;
double N = 100;
double M = 60;

double a(double x, double t) {
    return exp(t);
}

double a_t(double x, double t) {
    return exp(t);
}

double a_x(double x, double t) {
    return 0;
}

double b(double x, double t) {
    return t;
}

double b_t(double x, double t) {
    return 1;
}

double b_x(double x, double t) {
    return 0;
}

double u_t0(double x) {
    return (x-1) * (x-1);
}

double u_x0(double t) {
    return exp(2*t) + 0.5 * t * t;
}

double dudx(int x, int t) {
    return (3 * u[x][t] - 4 * u[x-1][t] + u[x-2][t]) / (2 * h);
}

double d2udx2(int x, int t) {
    return (u[x][t] - 2 * u[x-1][t] + u[x-2][t]) / (2 * h * h);
}

double alpha(double x, double t) {
    return b_t(x, t) - a(x, t) * b_x(x, t);
}

double beta(double x, double t) {
    return a(x, t) + 0.5 * tau * a_t(x, t) - 0.5 * tau * a_x(x, t) * a(x, t);
}

int main() {
    int k = 0;
    for (double p = 0; p <=1; p += h, k++) {
        u[k][0] = u_t0(p);
    }
    k = 0;
    for (double p = 0; p <=1; p += tau, k++) {
        u[0][k] = u_x0(p);
    }
    k = 0;
    for (double p = tau; p <= 1; p += tau, k++) {
        u[1][k] = exp(2 * p) + 0.5 * p * p - 2 * h * exp(p) - 2 * h * h;
    }

    int t = 1;
    for (double q = 0; q <= 1 - tau; q += tau, t++) {
        int x = 2;
        for (double p = 2 * h; p <= 1; p += h, x++) {
            u[x][t] = tau * (b(p, q) + 0.5 * tau * alpha(p, q) + a(p, q) * a(p, q) * tau * d2udx2(x, t-1) - beta(p, q) * dudx(x, t-1)) + u[x][t-1];
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            cerr << i * h << ' ' << j * tau << ' ' << u[i][j] << endl;
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            cout << u[i][j] << ' ';
        }
        cout << endl;
    }

    return 0;
}
