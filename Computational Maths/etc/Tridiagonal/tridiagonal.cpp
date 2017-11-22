#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <vector>

using namespace std;

int main() {
    // Ax = v
    vector<double> a, b, c, v;
    double a_, b_, c_, v_;
    int n;

    cin >> n;
    a.push_back(0);
    cin >> b_ >> c_;
    b.push_back(b_);
    c.push_back(c_);
    for (int i = 1; i < n - 1; ++i) {
        cin >> a_ >> b_ >> c_;
        a.push_back(a_);
        b.push_back(b_);
        c.push_back(c_);
    }
    cin >> a_ >> b_;
    a.push_back(a_);
    b.push_back(b_);
    for (int i = 0; i < n; ++i) {
        cin >> v_;
        v.push_back(v_);
    }

    c[0] = c[0] / b[0];
    v[0] = v[0] / b[0];
    for (int i = 1; i < n; ++i) {
        double m = 1.0 / (b[i] - a[i] * c[i - 1]);
        c[i] = c[i] * m;
        v[i] = (v[i] - a[i] * v[i - 1]) * m;
    }
    for (int i = n - 1; i-- > 0; ) {
        v[i] = v[i] - c[i] * v[i + 1];
    }

    for (int i = 0; i < n; ++i) {
        cout << v[i] << ' ';
    }
    cout << endl;
}
