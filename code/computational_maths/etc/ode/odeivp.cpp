#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>
#include <vector>
#include <time.h>

using namespace std;

#define buf_size 1024

int rk_S;
double* rk_a;
double** rk_b;
double* rk_c;
double** rk_k;
double tolerance;

double pr_alpha = 1.0;
double pr_beta = 2.0;
double pr_gamma = 1.0;
double pr_delta = 1.5;

double* y;
int N = 2;

void readMethod(int argc, char* argv[]);
bool checkIfExplicit();

void initialConditions(double* result) {
    result[0] = 1.0;
    result[1] = 0.05;
}

void computeF(double* result, double time, double* y) {
    result[0] = (pr_alpha - pr_beta * y[1]) * y[0];
    result[1] = (-pr_gamma + pr_delta * y[0]) * y[1];
}

double diffVectorLength(double* v1, double* v2) {
    return sqrt((v1[0] - v2[0]) * (v1[0] - v2[0]) + (v1[1] - v2[1]) * (v1[1] - v2[1]));
}

void printOutput(FILE* output, double time) {
    fprintf(output, "%f\t", time);
    int k;
    for (k = 0; k < N; k++) {
        fprintf(output, "%f\t", y[k]);
    }
    fprintf(output, "\n");
}

void computeImplicit() {
    double u[2], k1[2], k_temp[2], k_temp_ans[2];
    double tau = 0.001;
    vector<pair<double, double>> k_old, k_new;
    srand(time(0));
    k_old.assign(rk_S, make_pair((rand() % 1000) * 0.001, (rand() % 1000) * 0.001));
    k_new.assign(rk_S, make_pair((rand() % 1000) * 0.001, (rand() % 1000) * 0.001));
    FILE* output = fopen("output", "w");
    initialConditions(u);
    for (double t = 0; t < 500; t += 0.01) {
        k1[0] = 0;
        k1[1] = 0;
        double max_diff;
        do {
            max_diff = 0;
            double sum1 = 0, sum2 = 0;
            k_old = k_new;
            for (int i = 0; i < rk_S; ++i) {
                sum1 += rk_b[i][0] * k_old[i].first;
                sum2 += rk_b[i][0] * k_old[i].second;
            }
            for (int i = 0; i < rk_S; ++i) {
                k_temp[0] = u[0] + tau * sum1;
                k_temp[1] = u[1] + tau * sum2;
                computeF(k_temp_ans, t + rk_a[0], k_temp);
                k_temp[0] = k_old[i].first;
                k_temp[1] = k_old[i].second;

                k_new[i].first = k_temp_ans[0];
                k_new[i].second = k_temp_ans[1];

                double diff = diffVectorLength(k_temp, k_temp_ans);
                if (diff > max_diff) {
                    max_diff = diff;
                }
            }
        } while (max_diff > tolerance);
        for (int i = 0; i < rk_S; ++i) {
            k1[0] += tau * rk_c[i] * k_new[i].first;
            k1[1] += tau * rk_c[i] * k_new[i].second;
        }
        u[0] += k1[0];
        u[1] += k1[1];
        fprintf(output, "%f\t%f\t%f\n", t, u[0], u[1]);
    }
    fclose(output);
}

void computeExplicit() {
    double u[2], k[2], k1[2], t = 0;
    double tau = 0.001;
    FILE* output = fopen("output", "w");
    initialConditions(u);
    for (double t = 0; t < 500; t += 0.01) {
        k1[0] = 0;
        k1[1] = 0;
        for (int i = 0; i < rk_S; ++i) {
            computeF(k, t + rk_a[i], u);
            k1[0] += tau * rk_c[i] * k[0];
            k1[1] += tau * rk_c[i] * k[1];
        }
        u[0] += k1[0];
        u[1] += k1[1];
        fprintf(output, "%f\t%f\t%f\n", t, u[0], u[1]);
    }
    fclose(output);
}

int main(int argc, char* argv[]) {
    readMethod(argc, argv);
    int i;
    rk_k = (double**)calloc(rk_S, sizeof(double*));
    for (i = 0; i < rk_S; i++) {
        rk_k[i] = (double*)calloc(N, sizeof(double));
    }
    y = (double*)calloc(N, sizeof(double));
    if (checkIfExplicit()) {
        computeExplicit();
    } else {
        computeImplicit();
    }
    return 0;
}

void readMethod(int argc, char* argv[]) {
    FILE* input;
    if (argc < 2) {
        printf("Usage: %s <method filename>\n", argv[0]);
        exit(0);
    } else {
        input = fopen(argv[1], "r");
    }
    char buffer[buf_size];
    char* pch;
    fgets(buffer, buf_size, input);
    rk_S = atoi(buffer);
    int i, j;
    rk_a = (double*)calloc(rk_S, sizeof(double));
    rk_b = (double**)calloc(rk_S, sizeof(double*));
    for (i = 0; i < rk_S; i++) {
        rk_b[i] = (double*)calloc(rk_S, sizeof(double));
    }
    rk_c = (double*)calloc(rk_S, sizeof(double));
    for (i = 0; i < rk_S; i++) {
        fgets(buffer, buf_size, input);
        pch = strtok(buffer, " \t");
        rk_a[i] = atof(pch);
        for (j = 0; j < rk_S; j++) {
            pch = strtok(NULL, " \t");
            rk_b[i][j] = atof(pch);
        }
    }
    fgets(buffer, buf_size, input);
    pch = strtok(buffer, " \t");
    for (i = 0; i < rk_S; i++) {
        pch = strtok(NULL, " \t");
        rk_c[i] = atof(pch);
    }

    printf("The following RK method will be used:\n");
    printf("Number of stages: %d\n", rk_S);
    printf("Butcher's table:\n");
    for (i = 0; i < rk_S; i++) {
        printf("%6.4f  |  ", rk_a[i]);
        for (j = 0; j < rk_S; j++) {
            printf("%6.4f  ", rk_b[i][j]);
        }
        printf("\n");
    }
    printf("--------|--");
    for (i = 0; i < rk_S; i++) {
        printf("--------");
    }
    printf("\n        |  ");
    for (i = 0; i < rk_S; i++) {
        printf("%6.4f  ", rk_c[i]);
    }
    printf("\n");

    if (checkIfExplicit()) {
        printf("The method is explicit.\n");
    } else {
        printf("The method is implicit.\n");
        fgets(buffer, buf_size, input);
        tolerance = atof(buffer);
        printf("Tolerance: %f\n", tolerance);
    }
    fclose(input);
}

bool checkIfExplicit() {
    if (rk_a[0] != 0.0) {
        return false;
    }
    int i, j;
    for (i = 0; i < rk_S; i++) {
        for (j = i; j < rk_S; j++) {
            if (rk_b[i][j] != 0.0) {
                return false;
            }
        }
    }
    return true;
}
