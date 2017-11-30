#pragma once

#include "track_params.h"
#include "gnuplot_iostream.h"

using namespace std;

class GPlotter {
private:
    template <typename T>
    void SendArraysToCompare(const T& data, const vector<string>& names, const string& xlabel, const string& ylabel = "", const string& xtics = "auto");

    void PlotCompareAll(const vector<pair<vector<pair<TrackParams, string> >, string> >& data);
    void PlotCompareSets(const vector<pair<vector<pair<TrackParams, string> >, string> >& data, int meanMode);
    void Plot2DGraphs(const vector<pair<vector<pair<TrackParams, string> >, string> >& data, int clustersMode, int meanMode);

    Gnuplot gp;

public:
    GPlotter() {
        gp << "set terminal wxt size 1200,600 noenhanced" << endl;
    }

    void Plot(const vector<pair<vector<pair<TrackParams, string> >, string> >& data, int plottingMode, int clustersMode, int meanMode);

    void SwitchToPng(const string& name);
    void SwitchToWxt();
};
