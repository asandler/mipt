#include <iostream>
#include <vector>
#include <string>

using namespace std;

double lagr(vector<double> &x, vector<double> &y, double x_calc, size_t index) {
    double ans = 0.0;
    for (size_t i = index - 1; i < index + 3; ++i) {
        double temp = 1.0;
        for (size_t j = index - 1; j < index + 3; ++j) {
            if (i == j)
                continue;
            temp *= (x_calc - x[j]) / (x[i] - x[j]);
        }
        ans += y[i] * temp;
    }
    return ans;
}

int main() {
    vector<double> x, y;
    string s;
    while (!cin.eof()) {
        double x_cur, y_cur;
        cin >> x_cur >> y_cur;
        if (cin.eof())
            break;
        x.push_back(x_cur);
        y.push_back(y_cur);
    }

    for (size_t i = 1; i < x.size() - 2; ++i) {
        for (double x_calc = x[i]; x_calc <= x[i + 1]; x_calc += 0.001) {
            cout << lagr(x, y, x_calc, i) << ' ' << x_calc << endl;
        }
    }
    return 0;
}
