#include "ThreadSafeArrayViewer.hpp"


void ConcurrentApplication::ThreadSafeArrayViewer::loop(int dim1, int dim2) {
    if (dim1 <= 0 || dim2 <= 0) {
        std::cout << "Error: invalid arguments." << std::endl;
        return;
    }

    std::cout << "Welcome to Lab05 program!\n\n"
              << "\t1. Start.\n"
              << "\t2. Request.\n"
              << "\t0. Exit.\n";
    auto* app = new ThreadSafeArrayViewer(dim1, dim2);

    std::string input_command;

    while (true) {
        std::cout << "\nEnter command: ";
        std::getline(std::cin, input_command);

        if (input_command == "start") {
            app->start();
            std::cout << "Started." << std::endl;
        } else if (input_command == "request") {
            app->request();
        } else if (input_command == "exit") {
            if (!app) {
                delete app;
            }
            return;
        } else {
            std::cout << "Invalid command." << std::endl;
        }
    }
}
