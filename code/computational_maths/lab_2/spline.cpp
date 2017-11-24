#include <iostream>
#include <vector>

#define ld long double

using namespace std;


const ld h = 1e-2;

ld f(ld x) {
        return (x * (x + 10) - 1) / (40 * x * x + 1);
}

ld cube(ld a, ld b, ld c, ld d, ld x) {
        ld ans = a;
        ans = ans * x + b;
        ans = ans * x + c;
        return ans * x + d;
}


int main() {
        vector <ld> F, ap, bp, a, b, c, d;
        ap.insert(a.begin(), 2, 0);
        bp.insert(a.begin(), 2, 0);
        F.push_back(f(-1));
        for (ld i = -1 + h; i < 1; i += h) {
                F.push_back(f(i));
                bp.push_back((F.back() / h - bp.back()) / (ap.back() + 4));
                ap.push_back(-1 / (ap.back() + 4));
        }
        int n = F.size() - 1;
        a.assign(n + 2, 0);
        b.assign(n + 2, 0);
        c.assign(n + 2, 0);
        d.assign(n + 2, 0);

        c[n] = bp.back();
        c[1] = c[n + 1] = 0;
        for (int i = n - 1; i > 0; --i) {
                c[i] = ap[i + 1] * c[i + 1] + bp[i + 1];
                d[i] = (c[i + 1] - c[i]) / (3 * h);
                b[i] = (F[i] - F[i - 1]) / h - (h / 3) * (c[i + 1] + 2 * c[i]);
                a[i] = F[i - 1];
        }
        for (int i = 1; i < n; i++) {
                cout << cube(d[i], c[i], b[i], a[i], h / 2) << endl;
        }

        return 0;
}
