#include <iostream>
#include "ConcurrentRandomViewer.hpp"
using namespace std;

int main(int argc, char** argv)
{
    int dim, dim1, dim2;
    switch (argc) {
    case 1:
        std::cout << "Welcome to Lab05 application!\n"
                  << "Please start this application with two positive integers as arguments.\n\n"
                  << "Commands in-app:\n"
                  << "\t1. Start - start second thread to modify the 2D array.\n"
                  << "\t2. Request - show current state of the 2D array.\n"
                  << "\t0. Exit - Quit the program.\n\n" << std::endl;
        break;
    case 2:
        std::cout << "One argument supplied, second one is assumed to be the same. Starting the app...";
        dim = atoi(argv[1]);
        ThreadsTest::ConcurrentRandomViewer::main_loop(dim, dim);
        break;
    case 3:
        std::cout << "Two arguments supplied. Starting the app...";
        dim1 = atoi(argv[1]);
        dim2 = atoi(argv[2]);
        ThreadsTest::ConcurrentRandomViewer::main_loop(dim1, dim2);
        break;
    default:
        break;
    }
}
