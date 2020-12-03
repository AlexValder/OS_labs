#pragma once
#include <iostream>
#include <mutex>
#include <atomic>
#include <thread>

#define UNSAFE_NO_PROTECTION 0

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
#if !UNSAFE_NO_PROTECTION
        std::atomic_bool _started = false;
        std::atomic_uint _num_local_min = 0;
#else
        bool _started = false;
        unsigned _num_local_min = 0;
#endif

#if !UNSAFE_NO_PROTECTION
        std::mutex _lock;
#endif
        std::thread _modifer;

        ~ConcurrentRandomViewer();
    };
}
