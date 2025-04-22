#include "Application.h"
#include "Enums.h"
#include <iostream>

bool DEBUG = false;

int main() {

    // DEBUG Control
    DEBUG = true;
    if (DEBUG) {
        std::cout << "DEBUG ON" << std::endl;
    } else {
        std::cout << "DEBUG OFF" << std::endl;
    }

    Application app;

    return app.run();
}