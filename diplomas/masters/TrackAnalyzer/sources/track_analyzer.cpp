#include <sstream>

#include "track_analyzer.h"
#include "gplotter.h"
#include "helpers.h"

using namespace std;

bool CompareDataRecords(const pair<TrackParams, string>& a, const pair<TrackParams, string>& b) {
    return a.second < b.second;
}

double CalcIntegralDensityValueCoefficient(const pair<double, double>& p) {
    return sqrt(p.first / 100) * p.second;
}

double CalcVelocityDensityValueCoefficient(const pair<double, double>& p) {
    return sqrt(p.first / 10) / p.second;
}

double CalcVelocityIntegralValueCoefficient(const pair<double, double>& p) {
    return p.first / sqrt(p.second);
}

void AddTextTable(ostringstream& strs, const vector<string>& names, const string& title,
                 const string& col1Name, const vector<double>& value1,
                 const string& col2Name = "", const vector<double>& value2 = vector<double>(),
                 const string& col3Name = "", const vector<double>& value3 = vector<double>())
{
    strs << title << endl << "\t" << col1Name;
    if (col2Name != "")
        strs << "\t" << col2Name;
    if (col3Name != "")
        strs << "\t" << col3Name;
    strs << endl;

    for (size_t i = 0; i < names.size(); ++i) {
        if (names[i].length() > 8) {
            strs << names[i].substr(0, 8) << "..";
        } else {
            strs << names[i];
        }

        strs << "\t" << value1[i];
        if (col2Name != "")
            strs << "\t" << value2[i];
        if (col3Name != "")
            strs << "\t" << value3[i];
        strs << endl;
    }
    strs << endl;
}

void TrackAnalyzer::add(bool clearData) {
    if (clearData) {
        data.clear();
    }

    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open File(s).."), "", tr("Track Files (*.trk)"));

    if (fileNames.size() == 0) {
        return;
    }

    data.push_back(make_pair(vector<pair<TrackParams, string> >(), CalcSetLabelName(fileNames.begin()->toUtf8().constData())));
    size_t lastIndex = data.size() - 1;

    for (QStringList::iterator it = fileNames.begin(); it != fileNames.end(); ++it) {
        data[lastIndex].first.push_back(make_pair(GetTrackParams(it->toUtf8().constData()), CalcTrackLabelName(it->toUtf8().constData())));
    }

    sort(data[lastIndex].first.begin(), data[lastIndex].first.end(), CompareDataRecords);

    replot();
}

void TrackAnalyzer::addDirs(bool clearData) {
    if (clearData) {
        data.clear();
    }

    QFileDialog w;
    w.setFileMode(QFileDialog::DirectoryOnly);
    QListView *l = w.findChild<QListView*>("listView");
    if (l) {
        l->setSelectionMode(QAbstractItemView::MultiSelection);
    }
    QTreeView *t = w.findChild<QTreeView*>();
    if (t) {
        t->setSelectionMode(QAbstractItemView::MultiSelection);
    }

    if (w.exec()) {
        QStringList fileNames = w.selectedFiles();
        QStringList trackFiles;

        for (QStringList::iterator it = fileNames.begin(); it != fileNames.end(); ++it) {
            QDirIterator dirIt(it->toUtf8().constData(), QDirIterator::Subdirectories);

            data.push_back(make_pair(vector<pair<TrackParams, string> >(), CalcTrackLabelName(dirIt.path().toUtf8().constData())));
            size_t lastIndex = data.size() - 1;

            while (dirIt.hasNext()) {
                string curFileName = dirIt.next().toUtf8().constData();

                if (curFileName.substr(curFileName.length() - 3, 3) == "trk") {
                    data[lastIndex].first.push_back(make_pair(GetTrackParams(curFileName), CalcTrackLabelName(curFileName)));
                }
            }

            sort(data[lastIndex].first.begin(), data[lastIndex].first.end(), CompareDataRecords);
        }
        replot();
    }
}

void TrackAnalyzer::open() {
    add(true);
}

void TrackAnalyzer::openDirs() {
    addDirs(true);
}

void TrackAnalyzer::replot() {
    if (data.size() > 0) {
        plotter.Plot(data, GetPlottingMode(), GetClustersMode(), GetMeanMode());
        textEdit->setText(GetTracksTextInfo());
    }
}

void TrackAnalyzer::saveToPng() {
    if (data.size() > 0) {
        QString fileName = QFileDialog::getSaveFileName(this, "Save file", "", ".png");
        plotter.SwitchToPng(fileName.toUtf8().constData());
        plotter.Plot(data, GetPlottingMode(), GetClustersMode(), GetMeanMode());
        plotter.SwitchToWxt();
    } else {
        QMessageBox msgBox;
        msgBox.setText("Nothing to save. Select data via 'Open file(s)' and 'Open directory(-ies)' first");
        msgBox.exec();
    }
}

QString TrackAnalyzer::GetTracksTextInfo() {
    ostringstream strs;

    switch (GetPlottingMode()) {
        case 1:
            for (vector<pair<vector<pair<TrackParams, string> >, string> >::const_iterator setIt = data.begin(); setIt != data.end(); ++setIt) {
                strs << setIt->second << ":" << endl << endl;

                for (vector<pair<TrackParams, string> >::const_iterator it = setIt->first.begin(); it != setIt->first.end(); ++it) {
                    strs << "  " << it->second << endl;

                    strs << "    Integral: " << it->first.integral << endl;
                    strs << "    Missed gates: " << it->first.missed_gates << endl;
                    strs << "    Gates count: " << it->first.gates_count << endl;
                    strs << "    Mean density: " << it->first.dens_mean << endl;
                    strs << "    Mean density (among positive): " << it->first.dens_mean_plus << endl;
                    strs << "    Density dispersion: " << it->first.dens_dispersion << endl;
                    strs << "    Density dispersion (among positive): " << it->first.dens_dispersion_plus << endl;
                    strs << "    Mean velocity: " << it->first.velocity_mean << endl;
                    strs << "-----------------------------------------" << endl;
                }
                strs << "=========================================" << endl;
            }
            break;
        case 2:
            {
                vector<string> names;
                vector<vector<double> > values(12, vector<double>());

                for (vector<pair<vector<pair<TrackParams, string> >, string> >::const_iterator setIt = data.begin(); setIt != data.end(); ++setIt) {
                    for (size_t i = 0; i < 4; i++) {
                        values[i].push_back(1e9);
                    }
                    for (size_t i = 4; i < 12; i++) {
                        values[i].push_back(0);
                    }
                    size_t pointCount = 0, n = values[0].size() - 1;

                    for (vector<pair<TrackParams, string> >::const_iterator it = setIt->first.begin(); it != setIt->first.end(); ++it, ++pointCount) {
                        values[0][n] = min(values[0][n], it->first.integral);
                        values[1][n] = min(values[1][n], (double) it->first.missed_gates);
                        values[2][n] = min(values[2][n], (GetMeanMode() == 0 ? it->first.dens_mean : it->first.dens_mean_plus));
                        values[3][n] = min(values[3][n], it->first.velocity_mean);
                        values[4][n] = max(values[4][n], it->first.integral);
                        values[5][n] = max(values[5][n], (double) it->first.missed_gates);
                        values[6][n] = max(values[6][n], (GetMeanMode() == 0 ? it->first.dens_mean : it->first.dens_mean_plus));
                        values[7][n] = max(values[7][n], it->first.velocity_mean);
                        values[8][n] += it->first.integral;
                        values[9][n] += it->first.missed_gates;
                        values[10][n] += (GetMeanMode() == 0 ? it->first.dens_mean : it->first.dens_mean_plus);
                        values[11][n] += it->first.velocity_mean;
                    }

                    names.push_back(setIt->second);
                    values[8][n] /= pointCount;
                    values[9][n] /= pointCount;
                    values[10][n] /= pointCount;
                    values[11][n] /= pointCount;
                }

                AddTextTable(strs, names, "Integral stats:     ", "min", values[0], "max", values[4], "mean", values[8]);
                AddTextTable(strs, names, "Missed gates stats: ", "min", values[1], "max", values[5], "mean", values[9]);
                if (GetMeanMode() == 0) {
                    AddTextTable(strs, names, "Density stats:  ", "min", values[2], "max", values[6], "mean", values[10]);
                } else {
                    AddTextTable(strs, names, "Density+ stats: ", "min", values[2], "max", values[6], "mean", values[10]);
                }
                AddTextTable(strs, names, "Mean velocity stats:", "min", values[3], "max", values[7], "mean", values[11]);
            }
            break;
        case 3:
            {
                vector<string> names;
                vector<vector<double> > values(9, vector<double>());

                for (vector<pair<vector<pair<TrackParams, string> >, string> >::const_iterator setIt = data.begin(); setIt != data.end(); ++setIt) {
                    values[0].push_back(1e9);
                    for (size_t i = 1; i < 9; i++) {
                        values[i].push_back(0);
                    }

                    vector<pair<double, double> > integralDensity;
                    vector<pair<double, double> > velocityDensity;
                    size_t pointCount = 0, n = values[0].size() - 1;

                    for (vector<pair<TrackParams, string> >::const_iterator it = setIt->first.begin(); it != setIt->first.end(); ++it, ++pointCount) {
                        pair<double, double> newIntegralDensityPoint;
                        pair<double, double> newVelocityDensityPoint;
                        pair<double, double> newVelocityIntegralPoint;

                        newIntegralDensityPoint.first = it->first.integral;
                        newVelocityDensityPoint.first = it->first.velocity_mean;
                        newVelocityIntegralPoint.first = it->first.velocity_mean;
                        newVelocityIntegralPoint.second = it->first.integral;
                        if (GetMeanMode() == 0) {
                            newIntegralDensityPoint.second = it->first.dens_mean;
                            newVelocityDensityPoint.second = it->first.dens_mean;
                        } else {
                            newIntegralDensityPoint.second = it->first.dens_mean_plus;
                            newVelocityDensityPoint.second = it->first.dens_mean_plus;
                        }

                        values[0][n] = min(values[0][n], CalcIntegralDensityValueCoefficient(newIntegralDensityPoint));
                        values[2][n] = max(values[2][n], CalcVelocityDensityValueCoefficient(newVelocityDensityPoint));
                        values[4][n] = max(values[4][n], CalcVelocityIntegralValueCoefficient(newVelocityIntegralPoint));
                        values[1][n] += CalcIntegralDensityValueCoefficient(newIntegralDensityPoint);
                        values[3][n] += CalcVelocityDensityValueCoefficient(newVelocityDensityPoint);
                        values[5][n] += CalcVelocityIntegralValueCoefficient(newVelocityIntegralPoint);

                        integralDensity.push_back(newIntegralDensityPoint);
                        velocityDensity.push_back(newVelocityDensityPoint);
                    }

                    names.push_back(setIt->second);
                    values[1][n] /= pointCount;
                    values[3][n] /= pointCount;
                    values[5][n] /= pointCount;

                    pair<double, double> c1 = array_mean(integralDensity);
                    pair<double, double> c2 = array_mean(velocityDensity);
                    values[6][n] = c1.first;
                    values[7][n] = c1.second;
                    values[8][n] = c2.first;
                }

                AddTextTable(strs, names, "Integral-Density values (the lower - the better):  ", "best", values[0], "mean", values[1]);
                AddTextTable(strs, names, "Velocity-Density values (the bigger - the better): ", "best", values[2], "mean", values[3]);
                AddTextTable(strs, names, "Velocity-Integral values (the bigger - the better):", "best", values[4], "mean", values[5]); 

                if (GetClustersMode() == 1) {
                    AddTextTable(strs, names, "Cluster centers:", "Integral", values[6], "Density", values[7], "Velocity", values[8]);
                }
            }
            break;
        default:
            break;
    }

    return QString(strs.str().c_str());
}
