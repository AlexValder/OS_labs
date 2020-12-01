#include "ConcurrentRandomViewer.hpp"
#include <cstring>
using namespace ThreadsTest;

struct ConcurrentRandomViewer::crv_error : public std::runtime_error {
    crv_error(const char* what) : std::runtime_error(what) {}
};

ConcurrentRandomViewer::ConcurrentRandomViewer(size_t m, size_t n) : _dim1(m), _dim2(n) {
    if (m <= 0 || n <= 0) {
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
            this->_array[0][0] += 2;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });
}

void ConcurrentRandomViewer::request() noexcept {
    if (!this->_started) {
        std::cout << "Not started yet." << std::endl;
        return;
    }

    this->_lock.lock();
    for (size_t i = 0; i < this->_dim1; ++i) {
        for (size_t j = 0; j < this->_dim2; ++j) {
            std::cout << this->_array[i][j] << " ";
        }
        std::cout << "\b\n";
    }
    this->_lock.unlock();
}

void ConcurrentRandomViewer::stop() noexcept {
    this->_lock.lock();
    this->_started = false;
    this->_lock.unlock();
}
