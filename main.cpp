#include <iostream>
#include "Controller.hpp"

int main(int argc, char* argv[]) {
    Controller controller;
    try {
        controller.run(argc, argv);
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

