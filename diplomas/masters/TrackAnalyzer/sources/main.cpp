#include <QtGui/QtGui>
#include "track_analyzer.h"

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    TrackAnalyzer T;
    T.show();

    return app.exec();
}
