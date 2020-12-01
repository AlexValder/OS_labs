#pragma once
#include <iostream>
#include <mutex>
#include <atomic>
#include <thread>

namespace ThreadsTest {
    class ConcurrentRandomViewer final {
    public:
        ConcurrentRandomViewer(size_t m, size_t n);

        [[nodiscard]] inline int** array() const noexcept { return _array; }
        [[nodiscard]] inline int dim1() const noexcept { return _dim1; }
        [[nodiscard]] inline int dim2() const noexcept { return _dim2; }

        void start() noexcept;
        void request() noexcept;
        void stop() noexcept;

        static void main_loop(int dim1, int dim2);

        struct crv_error;

    private:
        int** _array;
        size_t _dim1, _dim2;
        std::atomic_bool _started = false;

        std::mutex _lock;
        std::thread _modifer;

        ~ConcurrentRandomViewer();
    };
}
