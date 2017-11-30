#pragma once

#include <QtGui/QtGui>

#include <string>
#include <vector>

#include "track_params.h"
#include "gplotter.h"

using namespace std;

class TrackAnalyzer: public QWidget {
    Q_OBJECT

public:
    TrackAnalyzer() {
        openButton = new QPushButton("Open file(s)..");
        addButton = new QPushButton("+");
        openDirsButton = new QPushButton("Open directory(-ies)..");
        addDirsButton = new QPushButton("+");
        saveToPngButton = new QPushButton("Save to PNG");

        openButton->setMinimumWidth(200);
        openDirsButton->setMinimumWidth(200);
        saveToPngButton->setMaximumWidth(150);

        connect(openButton, SIGNAL(clicked()), this, SLOT(open()));
        connect(addButton, SIGNAL(clicked()), this, SLOT(add()));
        connect(openDirsButton, SIGNAL(clicked()), this, SLOT(openDirs()));
        connect(addDirsButton, SIGNAL(clicked()), this, SLOT(addDirs()));
        connect(saveToPngButton, SIGNAL(clicked()), this, SLOT(saveToPng()));

        plottingModeGroup = new QGroupBox(tr("Plotting mode"));
        radio1 = new QRadioButton(tr("Compare all"));
        radio2 = new QRadioButton(tr("Compare sets"));
        radio3 = new QRadioButton(tr("2D graphs"));

        connect(radio1, SIGNAL(clicked()), this, SLOT(replot()));
        connect(radio2, SIGNAL(clicked()), this, SLOT(replot()));
        connect(radio3, SIGNAL(clicked()), this, SLOT(replot()));

        radio1->setChecked(true);

        densMeanMode = new QCheckBox(tr("Density+"));
        clusterCenters = new QCheckBox(tr("Cluster centers"));

        connect(densMeanMode, SIGNAL(clicked()), this, SLOT(replot()));
        connect(clusterCenters, SIGNAL(clicked()), this, SLOT(replot()));

        textEdit = new QTextEdit;
        textEdit->setMinimumWidth(380);

        layout = new QGridLayout;

        layout->addWidget(openButton, 0, Qt::AlignLeft);
        layout->addWidget(addButton, 0, Qt::AlignVCenter);
        layout->addWidget(openDirsButton, 1, Qt::AlignLeft);
        layout->addWidget(addDirsButton, 1, Qt::AlignVCenter);

        layout->addWidget(radio1, 2, Qt::AlignLeft);
        layout->addWidget(radio2, 3, Qt::AlignLeft);
        layout->addWidget(radio3, 4, Qt::AlignLeft);
        plottingModeGroup->setLayout(layout);

        layout->addWidget(densMeanMode, 2, Qt::AlignVCenter);
        layout->addWidget(clusterCenters, 3, Qt::AlignVCenter);
        layout->addWidget(saveToPngButton, 4, Qt::AlignVCenter);

        layout->addWidget(textEdit, 6, Qt::AlignLeft);

        setLayout(layout);

        setWindowTitle(tr("TrackAnalyzer"));
        setFixedSize(400, 500);
    }

private slots:
    void add(bool clearData = false);
    void addDirs(bool clearData = false);
    void open();
    void openDirs();

    void replot();

    void saveToPng();

private:
    QPushButton* openButton;
    QPushButton* addButton;
    QPushButton* openDirsButton;
    QPushButton* addDirsButton;

    QPushButton* saveToPngButton;
    QCheckBox* densMeanMode;
    QCheckBox* clusterCenters;

    QGroupBox* plottingModeGroup;
    QRadioButton *radio1;
    QRadioButton *radio2;
    QRadioButton *radio3;

    QTextEdit* textEdit;

    QGridLayout* layout;

    vector<pair<vector<pair<TrackParams, string> >, string> > data;
    GPlotter plotter;

    QString GetTracksTextInfo();

    int GetPlottingMode() {
        if (radio1->isChecked()) {
            return 1;
        } else if (radio2->isChecked()) {
            return 2;
        } else {
            return 3;
        }
    }

    int GetClustersMode() {
        if (clusterCenters->isChecked()) {
            return 1;
        } else {
            return 0;
        }
    }

    int GetMeanMode() {
        if (densMeanMode->isChecked()) {
            return 1;
        } else {
            return 0;
        }
    }
};
