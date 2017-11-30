#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

#include "track_params.h"
#include "helpers.h"

using namespace std;

TrackParams GetTrackParams(const string& filename) throw() {
    ifstream f(filename.c_str());
    string line;

    vector<vector<double> > data(6, vector<double>());
    vector<double> velocity_smooth;

    TrackParams params;
    double min_dens = 1000;

    if (f.is_open()) {
        getline(f, line);
        while (getline(f, line)) {
            istringstream iss(line);
            try {
                for (size_t i = 0; i < 5; i++) {
                    double val;
                    iss >> val;
                    data[i].push_back(val);
                }
                size_t n = data[0].size() - 1;
                if ((data[1][n] > 430) || (data[1][n] < -430)) {
                    continue;
                }
                min_dens = min(min_dens, data[4][n]);

                if (n > 0) {
                    velocity_smooth.push_back(eucl(data[0][n], data[0][n - 1], data[1][n], data[1][n - 1], data[2][n], data[2][n - 1]) / (data[3][n] - data[3][n - 1]));
                    if (velocity_smooth.size() > 500) {
                        params.velocity.push_back(array_mean(velocity_smooth, false, false, 0));
                        velocity_smooth.erase(velocity_smooth.begin());
                    }

                    if ((data[4][n] > 10) && (data[4][n - 1] < 10)) {
                        if (min_dens < 10) {
                            if (min_dens < 0) {
                                params.missed_gates++;
                                params.gates_miss.push_back(true);
                                params.density.push_back(make_pair(data[3][n - 1], -1));
                            } else {
                                params.density.push_back(make_pair(data[3][n - 1], min_dens));
                                params.gates_miss.push_back(false);
                            }
                            params.gates_count++;
                        }
                        min_dens = data[4][n];
                    }
                }
            } catch (exception& e) {
                cerr << e.what() << endl;
                throw e;
            }
        }

        params.dens_mean = array_mean(params.density, false, false, 1);
        params.dens_mean_plus = array_mean(params.density, true, false);

        params.dens_dispersion = array_mean(params.density, false, true, 1) - pow(params.dens_mean, 2);
        params.dens_dispersion_plus = array_mean(params.density, true, true) - pow(params.dens_mean_plus, 2);

        params.velocity_mean = array_mean(params.velocity, false, false);

        double l = params.density[0].second, r = 0, tl = params.density[0].first, tr = 0;
        for (size_t i = 1; i < params.density.size(); i++) {
            tr = params.density[i].first;
            r = params.density[i].second;

            if (l < 0) {
                l = 10;
            }
            if (r < 0) {
                r = 10;
            }

            params.integral += (l + fabs(r - l) / 2) * (tr - tl);

            tl = tr;
            l = r;
        }
    }

    return params;
}
