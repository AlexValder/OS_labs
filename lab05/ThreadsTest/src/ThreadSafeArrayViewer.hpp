#pragma once
#include <iostream>
#include <mutex>
#include <atomic>
#include <thread>
#include <cstring>

#define SAFETY_ON 0

namespace ConcurrentApplication {
    class ThreadSafeArrayViewer final {
    public:
        ThreadSafeArrayViewer(size_t m, size_t n) : _dim1(m), _dim2(n) {
            if (m <= 1 || n <= 1) {
                throw std::runtime_error("Invalid dimensions");
            }

            this->_array = new int*[m];
            for (size_t i = 0; i < m; i++) {
                this->_array[i] = new int[n];
                std::memset(this->_array[i], 0, n * sizeof(int));
            }
        }

        [[nodiscard]] inline int** array() const { return _array; }
        [[nodiscard]] inline int dim1() const { return _dim1; }
        [[nodiscard]] inline int dim2() const { return _dim2; }

        void start() {

            if (this->_started) {
                std::cout << "Already started." << std::endl;
                return;
            }

            this->_started = true;
            this->_thread = std::thread([&]() {
                while(this->_started) {
#if SAFETY_ON
                    this->_lock.lock();
#endif

                    for (size_t i = 0; i < this->_dim1; i++) {
                        for (size_t j = 0; j < this->_dim2; j++) {
                            this->_array[i][j] = rand() % 299 - 199; // from -200 to 200
                        }
                    }

                    auto* local_minimums = new int[this->_dim1];


                    for (size_t i = 0; i < this->_dim1; i++) {
                        local_minimums[i] = this->_array[i][0];
                        for (size_t j = 1; j < this->_dim2; j++) {
                            if (local_minimums[i] > this->_array[i][j]) {
                                local_minimums[i] = this->_array[i][j];
                            }
                        }
                    }

                    this->_max_of_mins = local_minimums[0];
                    for (size_t i = 0; i < this->_dim1; i++) {
                        if (this->_max_of_mins < local_minimums[i]) {
                            this->_max_of_mins = local_minimums[i];
                        }
                    }

#if SAFETY_ON
                    this->_lock.unlock();
#endif
                    std::this_thread::sleep_for(std::chrono::seconds(rand() % 10));
                }
            });
        }
        void request() {
            if (!this->_started) {
                std::cout << "Not started yet." << std::endl;
                return;
            }

#if SAFETY_ON
            this->_lock.lock();
#endif
            std::cout << "\nCurrent state of the 2D array:\n";
            for (size_t i = 0; i < this->_dim1; ++i) {
                for (size_t j = 0; j < this->_dim2; ++j) {
                    printf(" %3d ", this->_array[i][j]);
                }
                std::cout << "\b\n";
            }

            std::cout << "The biggest minimum of row minimums: " << this->_max_of_mins << std::endl;
#if SAFETY_ON
            this->_lock.unlock();
#endif
        }
        void stop() {
#if SAFETY_ON
            this->_lock.lock();
#endif
            this->_started = false;
#if SAFETY_ON
            this->_lock.unlock();
#endif
        }

        static void loop(int dim1, int dim2);

    private:
        int** _array;
        size_t _dim1, _dim2;
#if SAFETY_ON
        std::atomic_bool _started = false;
        std::atomic_int _max_of_mins = 0;
#else
        bool _started = false;
        int _max_of_mins = 0;
#endif

#if SAFETY_ON
        std::mutex _lock;
#endif
        std::thread _thread;

        ~ThreadSafeArrayViewer() {
#if SAFETY_ON
            this->_lock.lock();
#endif

            this->_started = false;
            this->_thread.join();

            for (size_t i = 0; i < this->_dim1; ++i) {
                delete[] this->_array[i];
            }
            delete[] this->_array;

#if SAFETY_ON
            this->_lock.unlock();
#endif
        }
    };
}
