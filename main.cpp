#include <cstdlib>
#include <exception>
#include "Application.h"

int main() {
    Application app;
    try {
        app.runApplicaton();
    } catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}