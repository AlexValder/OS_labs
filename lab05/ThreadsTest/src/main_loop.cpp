#include "ConcurrentRandomViewer.hpp"

enum class Options : size_t {
    Invalid, Start, Request, Exit
};

static Options str2enum(std::string& str) {
    if (str == "request") return Options::Request;
    if (str == "start") return Options::Start;
    if (str == "exit") return Options::Exit;
    return Options::Invalid;
}

void ThreadsTest::ConcurrentRandomViewer::main_loop(int dim1, int dim2) {
    if (dim1 <= 0 || dim2 <= 0) {
        std::cout << "Error: invalid arguments." << std::endl;
        return;
    }

    std::cout << "Welcome to Lab05 program!\n\n"
              << "\t1. Start.\n"
              << "\t2. Request.\n"
              << "\t0. Exit.\n";
    auto* crv = new ConcurrentRandomViewer(dim1, dim2);

    std::string input_command;

    while (true) {
        std::cout << "\nEnter command: ";
        std::getline(std::cin, input_command);

        switch (str2enum(input_command)) {
        case Options::Start:
            crv->start();
            std::cout << "Started." << std::endl;
            break;
        case Options::Request:
            crv->request();
            break;
        case Options::Exit:
            delete crv;
            return;
        default:
            std::cout << "Invalid command." << std::endl;
            break;
        }
    }
}
