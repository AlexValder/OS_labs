#include "ConcurrentRandomViewer.hpp"
#include <cstring>
using namespace ThreadsTest;

struct ConcurrentRandomViewer::crv_error : public std::runtime_error {
    crv_error(const char* what) : std::runtime_error(what) {}
};

ConcurrentRandomViewer::ConcurrentRandomViewer(size_t m, size_t n) : _dim1(m), _dim2(n) {
    if (m <= 1 || n <= 1) {
        throw crv_error("Invalid dimensions");
    }

    this->_array = new(std::nothrow) int*[m];
    if (!this->_array) throw std::bad_alloc();

    for (size_t i = 0; i < m; ++i) {
        this->_array[i] = new(std::nothrow) int[n];

        if (!this->_array[i]) {
            for (size_t j = 0; j < i; ++j) {
                delete[] this->_array[j];
            }
            throw std::bad_alloc();
        }

        std::memset(this->_array[i], 0, n * sizeof(int));
    }
}

ConcurrentRandomViewer::~ConcurrentRandomViewer() {

    this->_lock.lock();

    this->_started = false;
    this->_modifer.join();

    for (size_t i = 0; i < this->_dim1; ++i) {
        delete[] this->_array[i];
    }
    delete[] this->_array;

    this->_lock.unlock();
}

void ConcurrentRandomViewer::start() noexcept {

    if (this->_started) {
        std::cout << "Already started." << std::endl;
        return;
    }

    this->_started = true;
    this->_modifer = std::thread([&]() {
        while(this->_started) {
            this->_lock.lock();

            for (size_t i = 0; i < this->_dim1; ++i) {
                for (size_t j = 0; j < this->_dim2; ++j) {
                    this->_array[i][j] = rand() % 200 - 100; // from -100 to 100
                }
            }

            this->_num_local_min = 0;

            /*
             * Affected:
             * [ ][ ][ ][ ][ ]
             * [ ][x][x][x][ ]
             * [ ][x][x][x][ ]
             * [ ][x][x][x][ ]
             * [ ][ ][ ][ ][ ]
             */

            for (size_t i = 1; i < this->_dim1 - 1; ++i) {
                for (size_t j = 1; j < this->_dim2 - 1; ++j) {
                    if (
                          this->_array[i][j] < this->_array[i - 1][j] &&
                          this->_array[i][j] < this->_array[i + 1][j] &&
                          this->_array[i][j] < this->_array[i][j - 1] &&
                          this->_array[i][j] < this->_array[i][j + 1]
                            ) {
                        ++this->_num_local_min;
                    }
                }
            }

            /*
             * Affected:
             * [ ][ ][ ][ ][ ]
             * [x][ ][ ][ ][x]
             * [x][ ][ ][ ][x]
             * [x][ ][ ][ ][x]
             * [ ][ ][ ][ ][ ]
             */

            for (size_t i = 1; i < this->_dim1 - 1; ++i) {
                if (
                      this->_array[i][0] < this->_array[i - 1][0] &&
                      this->_array[i][0] < this->_array[i + 1][0] &&
                      this->_array[i][0] < this->_array[i][1]
                        ) {
                    ++this->_num_local_min;
                }
                if (
                      this->_array[i][this->_dim1 - 1] < this->_array[i - 1][this->_dim1 - 1] &&
                      this->_array[i][this->_dim1 - 1] < this->_array[i + 1][this->_dim1 - 1] &&
                      this->_array[i][this->_dim1 - 1] < this->_array[i][this->_dim1 - 2]
                        ) {
                    ++this->_num_local_min;
                }
            }

            /*
             * Affected:
             * [ ][x][x][x][ ]
             * [ ][ ][ ][ ][ ]
             * [ ][ ][ ][ ][ ]
             * [ ][ ][ ][ ][ ]
             * [ ][x][x][x][ ]
             */
            for (size_t j = 1; j < this->_dim2 - 1; ++j) {
                if (
                      this->_array[0][j] < this->_array[0][j - 1] &&
                      this->_array[0][j] < this->_array[0][j + 1] &&
                      this->_array[0][j] < this->_array[1][j]
                        ) {
                    ++this->_num_local_min;
                }
                if (
                      this->_array[this->_dim1 - 1][j] < this->_array[this->_dim1 - 1][j - 1] &&
                      this->_array[this->_dim1 - 1][j] < this->_array[this->_dim1 - 1][j + 1] &&
                      this->_array[this->_dim1 - 1][j] < this->_array[this->_dim1 - 2][j]
                        ) {
                    ++this->_num_local_min;
                }
            }

            /*
             * Affected:
             * [x][ ][ ][ ][x]
             * [ ][ ][ ][ ][ ]
             * [ ][ ][ ][ ][ ]
             * [ ][ ][ ][ ][ ]
             * [x][ ][ ][ ][x]
             */

            if (
                  this->_array[0][0] < this->_array[0][1] &&
                  this->_array[0][0] < this->_array[1][0]
                    ) {
                ++this->_num_local_min;
            }

            if (
                  this->_array[0][this->_dim2 - 1] < this->_array[0][this->_dim2 - 2] &&
                  this->_array[0][this->_dim2 - 1] < this->_array[1][this->_dim2 - 1]
                    ) {
                ++this->_num_local_min;
            }

            if (
                  this->_array[this->_dim1 - 1][this->_dim2 - 1] < this->_array[this->_dim1 - 1][this->_dim2 - 2] &&
                  this->_array[this->_dim1 - 1][this->_dim2 - 1] < this->_array[this->_dim1 - 2][this->_dim2 - 1]
                    ) {
                ++this->_num_local_min;
            }

            if (
                  this->_array[this->_dim1 - 1][0] < this->_array[this->_dim1 - 1][1] &&
                  this->_array[this->_dim1 - 1][0] < this->_array[this->_dim1 - 2][0]
                    ) {
                ++this->_num_local_min;
            }

            this->_lock.unlock();
            std::this_thread::sleep_for(std::chrono::seconds(rand() % 10));
        }
    });
}

void ConcurrentRandomViewer::request() noexcept {
    if (!this->_started) {
        std::cout << "Not started yet." << std::endl;
        return;
    }

    this->_lock.lock();
    std::cout << "\nCurrent state of the 2D array:\n";
    for (size_t i = 0; i < this->_dim1; ++i) {
        for (int q = 0; q < (int)this->_dim1 * 6; ++q) putchar('-');
        printf("+\n");
        for (size_t j = 0; j < this->_dim2; ++j) {
            printf("| %3d ", this->_array[i][j]);
            fflush(nullptr);
        }
        std::cout << "|\b\n";
    }
    for (int q = 0; q < (int)this->_dim1 * 6; ++q) putchar('-');
    printf("+\n");

    std::cout << "Number of local minimums: " << this->_num_local_min << std::endl;
    this->_lock.unlock();
}

void ConcurrentRandomViewer::stop() noexcept {
    this->_lock.lock();
    this->_started = false;
    this->_lock.unlock();
}
