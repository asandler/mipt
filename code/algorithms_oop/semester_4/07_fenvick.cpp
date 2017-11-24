#include <iostream>
#include <vector>

using namespace std;

class FenwickTree {
public:
    FenwickTree(int _n, int _m) {
        t.assign(_n, vector<int>(_m, 0));
        n = _n;
        m = _m;
    }
    int Sum(int left, int right, int bottom, int top) {
        return sum(right, top) - sum(left - 1, top) - sum(right, bottom - 1) + sum(left - 1, bottom - 1);
    }

    void Inc(int x, int y, int delta) {
        for (int i = x; i < n; i = (i | (i + 1))) {
            for (int j = y; j < m; j = (j | (j + 1))) {
                t[i][j] += delta;
            }
        }
    }
    void Print() {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                cout << t[i][j] << "    ";
            }
            cout << endl;
        }
    }

private:
    int sum(int x, int y) {
        int result = 0;
        for (int i = x; i >= 0; i = (i & (i + 1)) - 1) {
            for (int j = y; j >= 0; j = (j & (j + 1)) - 1) {
                result += t[i][j];
            }
        }
        return result;
    }

    vector<vector<int>> t;
    int n, m;
};

int main() {
    FenwickTree T(10, 10);
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            T.Inc(i, j, 1);
        }
    }
    //        T.Print();
    cout << T.Sum(3, 5, 3, 5) << endl;
    return 0;
}
