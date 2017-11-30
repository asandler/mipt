#pragma once

#include <string>
#include <vector>

using namespace std;

double eucl(double x1, double x2, double y1, double y2, double z1, double z2);

string CalcSetLabelName(const string& s);
string CalcTrackLabelName(const string& s);

double array_mean(const vector<double>& v, bool ge_zero, bool calc_squares, double setNegToConst = 0);
double array_mean(const vector<pair<double, double> >& v, bool ge_zero, bool calc_squares, double setNegToConst = 0);
pair<double, double> array_mean(const vector<pair<double, double> >& v);
