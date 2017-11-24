#include <iostream>
using namespace std;

void input(int& n) {
    cout << "This program calculates Fibonacci numbers\n";
    cout << "Enter n = ";
    cin >> n;
}

long long f(int t) {
    long long a = 0;
    long long b = 1;
    long long c = 1;
    for (int i = 0; i < t; ++i) {
        c = a + b;
        a = b;
        b = c;
    }
    return c;
}

int main() {
    int n;
    input(n);
    cout << f(n) << " is " << n << " Fibonacci number\n";
    getchar();
    return 0;
}