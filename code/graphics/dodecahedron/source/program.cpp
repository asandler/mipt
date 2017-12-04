#include "application.h"

int main() {
    Application app;
    srand(time(0));

    app.initContext();
    app.initGL();
    app.makeScene();
    app.run();

    return 0;
}
