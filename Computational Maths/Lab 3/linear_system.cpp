#include <iostream>
#include <map>
#include <math.h>

using namespace std;

const double eps = 1e-8;

class LinearSystemSolver {
public:
    void Init() {
        for (int i = 0; i < 11; i++) {
            for (int j = 0; j < 11; j++) {
                int cur = i * 11 + j;
                x[cur] = j;
                if (i > 2 && i < 7 && j > 2 && j < 7) {
                    k[make_pair(cur, cur + 1)] = 5;
                    k[make_pair(cur, cur + 11)] = 5;
                } else {
                    k[make_pair(cur, cur + 1)] = 1;
                    k[make_pair(cur, cur + 11)] = 1;
                }
            }
        }
        for (int i = 0; i < 121; i++) {
            fixedPoints[i] = -1;
            g[i] = 0;
        }
        fixedPoints[0] = 0;
        fixedPoints[10] = 10;
        fixedPoints[110] = 0;
        fixedPoints[120] = 10;
        for (int i = 0; i < 121; i++) {
            for (int j = 0; j < 121; j++) {
                matrix[i][j] = 0;
            }
        }
    }

    void MakeLinearSystem() {
        for (int i = 0; i < 11; i++) {
            for (int j = 0; j < 11; j++) {
                int cur = i * 11 + j;
                if (fixedPoints[cur] == -1) {
                    if (i > 0) {
                        matrix[cur][cur] += k[make_pair(cur - 11, cur)];
                        if (fixedPoints[cur - 11] == -1) {
                            matrix[cur][cur - 11] -= k[make_pair(cur - 11, cur)];
                        } else {
                            g[cur] += k[make_pair(cur - 11, cur)] * fixedPoints[cur - 11];
                        }
                    }
                    if (i < 10) {
                        matrix[cur][cur] += k[make_pair(cur, cur + 11)];
                        if (fixedPoints[cur + 11] == -1) {
                            matrix[cur][cur + 11] -= k[make_pair(cur, cur + 11)];
                        } else {
                            g[cur] += k[make_pair(cur, cur + 11)] * fixedPoints[cur + 11];
                        }
                    }
                    if (j > 0) {
                        matrix[cur][cur] += k[make_pair(cur - 1, cur)];
                        if (fixedPoints[cur - 1] == -1) {
                            matrix[cur][cur - 1] -= k[make_pair(cur - 1, cur)];
                        } else {
                            g[cur] += k[make_pair(cur - 1, cur)] * fixedPoints[cur - 1];
                        }
                    }
                    if (j < 10) {
                        matrix[cur][cur] += k[make_pair(cur, cur + 1)];
                        if (fixedPoints[cur + 1] == -1) {
                            matrix[cur][cur + 1] -= k[make_pair(cur, cur + 1)];
                        } else {
                            g[cur] += k[make_pair(cur, cur + 1)] * fixedPoints[cur + 1];
                        }
                    }
                }
            }
        }
        matrix[0][0] = 1;
        matrix[10][10] = 1;
        matrix[110][110] = 1;
        matrix[120][120] = 1;
        g[0]= 0;
        g[10]= 10;
        g[110]= 0;
        g[120]= 10;
    }

    void Print() {
        /*
        for (int i = 0; i < 121; i++) {
            for (int j = 0; j < 121; j++) {
                cout << matrix[i][j] << ' ';
            }
            cout << g[i] << endl;
        }
        */
        for (int i = 0; i < 11; i++) {
            for (int j = 0; j < 11; j++) {
                int cur = i * 11 + j;
                int trn = j * 11 + i;
                cout << x[cur] << ' ' << x[trn] << endl;
            }
        }
    }

    void Jacobi() {
        double norm = 1;
        while (norm > eps) {
            for (int i = 0; i < 121; i++) {
                tempx[i] = g[i];
                for (int j = 0; j < 121; j++) {
                    if (i != j) {
                        tempx[i] -= matrix[i][j] * x[j];
                    }
                }
                tempx[i] /= matrix[i][i];
            }
            norm = fabs(x[0] - tempx[0]);
            for (int i = 0; i < 121; i++) {
                if (fabs(x[i] - tempx[i]) > norm) {
                    norm = fabs(x[i] - tempx[i]);
                }
                x[i] = tempx[i];
            }
            //cout << norm << endl;
        }
    }

private:
    map<pair<int, int>, int> k;
    map<int, int> fixedPoints;
    double matrix[121][121];
    double g[121], x[121], tempx[121];
};

int main() {
    LinearSystemSolver S;
    S.Init();
    S.MakeLinearSystem();
    S.Jacobi();
    S.Print();
    return 0;
}
