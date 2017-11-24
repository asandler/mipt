#include <iostream>
#include <ctime>
using namespace std;

int a[30];
int n = 30;

int sorted() {
    int p = 1;
    for (int i = 0; i < n - 1; ++i)
        if (a[i] > a[i + 1])
            p = 0;
    return p;
}

void rando() {
    for (int i = 0; i < n; ++i)
        a[i] = rand();
}

void swap(int& a, int& b) {
    int c = a;
    a = b;
    b = c;
}

void puzir() {
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n - i - 1; ++j)
            if (a[j] > a[j + 1])
                swap(a[j], a[j + 1]);
}

void output() {
    cout << n << endl;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 10; ++j)
            cout << a[i * 10 + j] << " ";
        cout << endl;
    }
    cout << endl;
}

void main() {
    srand(time(0));
    rando();
    output();
    puzir();
    output();
    if (sorted())
        cout << "It works\n";
    else
        cout << "It doesn't works\n";
    getchar();
}