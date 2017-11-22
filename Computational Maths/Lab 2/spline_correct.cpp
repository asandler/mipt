#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <vector>

#define ld float

using namespace std;

vector<ld> x, f, s, F;
vector<vector<ld> > m;
int n = -1;
ld h;

ld func(ld x) {
        //return (x * (x + 10) - 1) / (40 * x * x + 1);
        return (-400 * x * x + 82 * x + 10) / (1600 * pow(x, 4) + 80 * x * x + 1);
}

ld cube(ld a, ld b, ld c, ld d, ld x) {
        ld ans = a;
        ans = ans * x + b;
        ans = ans * x + c;
        return ans * x + d;
}

ld interpolate(ld p) {
        ld sum, a, b, c, d;
        for (int i = 0; i < n - 1; i++) {
                if (x[i] <= p && p <= x[i + 1]) {
                        a = (s[i + 1] - s[i]) / (6 * h);
                        b = s[i] / 2;
                        c = (f[i + 1] - f[i]) / h - (2 * h * s[i] + s[i + 1] * h) / 6;
                        d = f[i];
                        sum = cube(a, b, c, d,  p - x[i]);
                }
        }
        return sum;
}

int main() {
        cin >> h;
        for (float z = -1 - h; z < 1 + h; z += h, n++) {
                x.push_back(z);
                f.push_back(func(z));
        }

        s.assign(n + 2, 0);
        F.assign(n + 2, 0);
        m.assign(n + 2, vector<ld>(n + 2, 0));

        for (int i = 1; i < n; i++) {
                F[i] = (f[i] - f[i - 1]) / h;
        }

        //*********** formation of h, s , f matrix **************//
        for (int i = 1; i < n - 1; i++) {
                m[i][i] = 4 * h;
                if (i != 1) {
                        m[i][i - 1] = m[i - 1][i] = h;
                }
                m[i][n - 1] = 6 * (F[i + 1] - F[i]);
        }

        //***********  forward elimination **************//

        for(int i = 1; i < n - 2; i++) {
                ld temp = m[i + 1][i] / m[i][i];
                for (int j = 1; j <= n - 1; j++) {
                        m[i + 1][j] -= temp * m[i][j];
                }
        }

        //*********** back ward substitution *********//
        for (int i = n - 2; i > 0; i--) {
                ld sum = 0;
                for (int j = i; j <= n - 2; j++) {
                        sum += m[i][j] * s[j];
                }
                s[i] = (m[i][n - 1] - sum) / m[i][i];
        }

        ld ans = 0;
        for (ld p = -1; p < 1; p += h / 100) {
                cout << p << "\t" << interpolate(p) << "\t" << func(p) << endl;
                ld i = interpolate(p);
                ld f = func(p);
                ans = (i > f) ? max(ans, i - f) : max(ans, f - i);
        }
        //cout << ans << endl;
        return 0;
}
