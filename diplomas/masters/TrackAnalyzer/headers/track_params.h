#pragma once

#include <string>
#include <vector>

using namespace std;

struct TrackParams {
    TrackParams()
        : integral(0)
        , missed_gates(0)
        , gates_count(0)
        , dens_mean(0)
        , dens_mean_plus(0)
        , dens_dispersion(0)
        , dens_dispersion_plus(0)
        , velocity_mean(0)
    {
        density.clear();
        velocity.clear();
    }

    vector<pair<double, double> > density;
    vector<double> velocity;
    vector<bool> gates_miss;

    double integral;

    size_t missed_gates;
    size_t gates_count;

    double dens_mean;
    double dens_mean_plus;

    double dens_dispersion;
    double dens_dispersion_plus;

    double velocity_mean;
};

TrackParams GetTrackParams(const string& filename) throw();
