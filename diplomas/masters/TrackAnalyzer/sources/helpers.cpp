#include "helpers.h"
#include <cmath>

using namespace std;

double eucl(double x1, double x2, double y1, double y2, double z1, double z2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2));
}

string CalcSetLabelName(const string& s) {
    size_t rightMost = s.rfind('/');
    size_t nextRight = s.substr(0, rightMost - 1).rfind('/');
    return s.substr(nextRight + 1, rightMost - nextRight - 1);
}

string CalcTrackLabelName(const string& s) {
    return s.substr(s.rfind('/') + 1);
}

double array_mean(const vector<double>& v, bool ge_zero, bool calc_squares, double setNegToConst) {
    size_t n = 0;
    double s = 0;
    for (size_t i = 0; i < v.size(); i++) {
        if (v[i] >= 0) {
            if (calc_squares) {
                s += v[i] * v[i];
            } else {
                s += v[i];
            }
            n++;
        } else {
            if (!ge_zero) {
                if (calc_squares) {
                    s += setNegToConst * setNegToConst;
                } else {
                    s += setNegToConst;
                }
                n++;
            }
        }
    }
    return s / n;
}

double array_mean(const vector<pair<double, double> >& v, bool ge_zero, bool calc_squares, double setNegToConst) {
    size_t n = 0;
    double s = 0;
    for (size_t i = 0; i < v.size(); i++) {
        if (v[i].second >= 0) {
            if (calc_squares) {
                s += v[i].second * v[i].second;
            } else {
                s += v[i].second;
            }
            n++;
        } else {
            if (!ge_zero) {
                if (calc_squares) {
                    s += setNegToConst * setNegToConst;
                } else {
                    s += setNegToConst;
                }
                n++;
            }
        }
    }
    return s / n;
}

pair<double, double> array_mean(const vector<pair<double, double> >& v) {
    double l = 0, r = 0;
    for (size_t i = 0; i < v.size(); i++) {
        l += v[i].first;
        r += v[i].second;
    }
    return make_pair(l / v.size(), r / v.size());
}
