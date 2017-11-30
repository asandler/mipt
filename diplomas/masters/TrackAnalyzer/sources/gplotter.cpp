#include "gplotter.h"
#include "helpers.h"

using namespace std;

template <typename T>
void GPlotter::SendArraysToCompare(const T& data, const vector<string>& names, const string& xlabel, const string& ylabel, const string& xtics) {
    gp << "set xlabel '" << xlabel << "';" << endl;
    gp << "set ylabel '" << ylabel << "';" << endl;
    gp << "set xtics " << xtics << ";" << endl;
    gp << "plot ";

    size_t i = 0, color = 1;
    for (vector<string>::const_iterator it = names.begin(); it != names.end(); ++it, ++i) {
        if (names[i].find(" mean") != string::npos) {
            gp << "'-' with points pt 7 ps 2 linecolor " << color - 1 << " title '" << *it << ((i + 1 == names.size()) ? "';\n" : "', ");
        } else {
            if (data[i].size() == 1) {
                gp << "'-' with points pt 7 ps 1 linecolor " << color++ << " title '" << *it << ((i + 1 == names.size()) ? "';\n" : "', ");
            } else {
                gp << "'-' with lines linecolor " << color++ << " title '" << *it << ((i + 1 == names.size()) ? "';\n" : "', ");
            }
        }
    }
    for (typename T::const_iterator it = data.begin(); it != data.end(); ++it) {
        gp.send1d(*it);
    }
}

void GPlotter::PlotCompareAll(const vector<pair<vector<pair<TrackParams, string> >, string> >& data) {
    gp << "set multiplot layout 1,3 rowsfirst; set autoscale" << endl;
    gp << "set lmargin at screen 0.05; set rmargin at screen 0.7" << endl;

    vector<vector<pair<double, double> > > density;
    vector<vector<double> > velocity;
    vector<string> names;

    for (vector<pair<vector<pair<TrackParams, string> >, string> >::const_iterator setIt = data.begin(); setIt != data.end(); ++setIt) {
        for (vector<pair<TrackParams, string> >::const_iterator it = setIt->first.begin(); it != setIt->first.end(); ++it) {
            density.push_back(it->first.density);
            velocity.push_back(it->first.velocity);
            names.push_back(setIt->second + "/" + it->second);
        }
    }

    gp << "unset key; set size 1, 0.5; set origin 0, 0.5" << endl;
    SendArraysToCompare(density, names, "Density");
    gp << "set size 1, 0.5; set origin 0, 0.0" << endl;
    SendArraysToCompare(velocity, names, "Velocity");

    gp << "set lmargin at screen 0.75; set rmargin at screen 0.98;" << endl;
    gp << "set key vertical at screen 0.98, screen 0.95 samplen 1 maxrows 40 maxcols 1; set yrange [-1:18]; set xlabel ''" << endl;
    string s = "plot";

    gp << "unset border; unset tics" << endl;
    gp << "plot";
    size_t i = 0;
    for (vector<string>::iterator it = names.begin(); it != names.end(); ++it, ++i) {
        gp << " NaN title '" << *it << ((i + 1 == names.size()) ? "';\n" : "', ");
    }

    gp << "unset lmargin; unset rmargin" << endl;
    gp << "set border; set tics" << endl;
    gp << "unset multiplot" << endl;
}

void GPlotter::PlotCompareSets(const vector<pair<vector<pair<TrackParams, string> >, string> >& data, int meanMode) {
    gp << "set multiplot layout 2,2 rowsfirst; set autoscale" << endl;

    vector<vector<double> > integrals;
    vector<vector<double> > missed_gates_prob;
    vector<vector<double> > missed_gates_prob_smooth;
    vector<vector<double> > density;
    vector<vector<double> > velocity_mean;
    vector<string> names;

    for (vector<pair<vector<pair<TrackParams, string> >, string> >::const_iterator setIt = data.begin(); setIt != data.end(); ++setIt) {
        size_t gates_count = setIt->first.begin()->first.gates_miss.size();
        size_t set_size = setIt->first.size();

        integrals.push_back(vector<double>());
        missed_gates_prob.push_back(vector<double>(gates_count, 0));
        missed_gates_prob_smooth.push_back(vector<double>(gates_count, 0));
        density.push_back(vector<double>());
        velocity_mean.push_back(vector<double>());

        size_t curIndex = integrals.size() - 1;
        names.push_back(setIt->second);

        for (vector<pair<TrackParams, string> >::const_iterator it = setIt->first.begin(); it != setIt->first.end(); ++it) {
            integrals[curIndex].push_back(it->first.integral);
            for (size_t i = 0; i < gates_count; ++i) {
                missed_gates_prob[curIndex][i] += int(it->first.gates_miss[i]);
            }
            if (meanMode == 0) {
                density[curIndex].push_back(it->first.dens_mean);
            } else {
                density[curIndex].push_back(it->first.dens_mean_plus);
            }
            velocity_mean[curIndex].push_back(it->first.velocity_mean);
        }
        for (size_t i = 0; i < gates_count; ++i) {
            missed_gates_prob[curIndex][i] /= set_size;
        }
        for (size_t i = 1; i < gates_count - 1; ++i) {
            missed_gates_prob_smooth[curIndex][i] = 0.1666 * missed_gates_prob[curIndex][i - 1] + 0.6666 * missed_gates_prob[curIndex][i] + 0.1666 * missed_gates_prob[curIndex][i + 1];
        }
        missed_gates_prob_smooth[curIndex][0] = 0.6666 * missed_gates_prob[curIndex][0] + 0.3333 * missed_gates_prob[curIndex][1];
        missed_gates_prob_smooth[curIndex][gates_count - 1] = 0.6666 * missed_gates_prob[curIndex][gates_count - 1] + 0.3333 * missed_gates_prob[curIndex][gates_count - 2];
    }

    gp << "set key at screen 0.47,screen 0.95" << endl;
    SendArraysToCompare(integrals, names, "Integrals", "", "1");
    gp << "set key at screen 0.97,screen 0.95" << endl;
    SendArraysToCompare(missed_gates_prob_smooth, names, "Gate miss probability");
    gp << "set key at screen 0.47,screen 0.45" << endl;
    if (meanMode == 0) {
        SendArraysToCompare(density, names, "Mean density", "", "1");
    } else {
        SendArraysToCompare(density, names, "Mean density+", "", "1");
    }
    gp << "set key at screen 0.97,screen 0.45" << endl;
    SendArraysToCompare(velocity_mean, names, "Mean velocity", "", "1");

    gp << "unset multiplot" << endl;
}

void GPlotter::Plot2DGraphs(const vector<pair<vector<pair<TrackParams, string> >, string> >& data, int clustersMode, int meanMode) {
    gp << "set multiplot layout 2,2 rowsfirst; set autoscale" << endl;

    vector<vector<pair<double, double> > > integralDensity;
    vector<vector<pair<double, double> > > velocityDensity;
    vector<vector<pair<double, double> > > velocityIntegral;
    vector<string> names;

    for (vector<pair<vector<pair<TrackParams, string> >, string> >::const_iterator setIt = data.begin(); setIt != data.end(); ++setIt) {
        names.push_back(setIt->second);
        integralDensity.push_back(vector<pair<double, double> >());
        velocityDensity.push_back(vector<pair<double, double> >());
        velocityIntegral.push_back(vector<pair<double, double> >());
        size_t lastIndex = integralDensity.size() - 1;

        for (vector<pair<TrackParams, string> >::const_iterator it = setIt->first.begin(); it != setIt->first.end(); ++it) {
            if (meanMode == 0) {
                integralDensity[lastIndex].push_back(make_pair(it->first.integral, it->first.dens_mean));
                velocityDensity[lastIndex].push_back(make_pair(it->first.velocity_mean, it->first.dens_mean));
            } else {
                integralDensity[lastIndex].push_back(make_pair(it->first.integral, it->first.dens_mean_plus));
                velocityDensity[lastIndex].push_back(make_pair(it->first.velocity_mean, it->first.dens_mean_plus));
            }
            velocityIntegral[lastIndex].push_back(make_pair(it->first.velocity_mean, it->first.integral));
        }

        if (clustersMode == 1) {
            names.push_back(setIt->second + " mean");
            integralDensity.push_back(vector<pair<double, double> >(1, array_mean(integralDensity[lastIndex])));
            velocityDensity.push_back(vector<pair<double, double> >(1, array_mean(velocityDensity[lastIndex])));
            velocityIntegral.push_back(vector<pair<double, double> >(1, array_mean(velocityIntegral[lastIndex])));
        }
    }

    gp << "unset key; set lmargin at screen 0.1; set rmargin at screen 0.45" << endl;
    if (meanMode == 0) {
        SendArraysToCompare(integralDensity, names, "Integral", "Mean Density");
        gp << "set lmargin at screen 0.55; set rmargin at screen 0.9" << endl;
        SendArraysToCompare(velocityDensity, names, "Mean velocity", "Mean Density");
    } else {
        SendArraysToCompare(integralDensity, names, "Integral", "Mean Density+");
        gp << "set lmargin at screen 0.55; set rmargin at screen 0.9" << endl;
        SendArraysToCompare(velocityDensity, names, "Mean velocity", "Mean Density+");
    }
    gp << "set lmargin at screen 0.1; set rmargin at screen 0.45; set key at screen 0.9,screen 0.48" << endl;
    SendArraysToCompare(velocityIntegral, names, "Mean velocity", "Integral");

    gp << "unset lmargin; unset rmargin; unset key" << endl;
    gp << "unset multiplot" << endl;
}

void GPlotter::Plot(const vector<pair<vector<pair<TrackParams, string> >, string> >& data, int plottingMode, int clustersMode, int meanMode) {
    switch (plottingMode) {
        case 1:
            return PlotCompareAll(data);
        case 2:
            return PlotCompareSets(data, meanMode);
        case 3:
            return Plot2DGraphs(data, clustersMode, meanMode);
        default:
            return;
    }
}

void GPlotter::SwitchToPng(const string& name) {
    gp << "set terminal png size 1200,600 noenhanced" << endl;
    gp << "set output '" << name << ".png'" << endl;
}

void GPlotter::SwitchToWxt() {
    gp << "set terminal wxt size 1200,600 noenhanced" << endl;
}
