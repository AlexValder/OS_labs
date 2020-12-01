#include <iostream>
#include "ConcurrentRandomViewer.hpp"
using namespace std;

int main()
{
    auto* crv = new ThreadsTest::ConcurrentRandomViewer(3, 5);

    cout << "Created object, dim1 = " << crv->dim1() << ", dim2 = " << crv->dim2() << endl;

    crv->start();
    for (int i = 0; i < 5; ++i) {
        cout << "Request " << i + 1 << endl;
        crv->request();
        this_thread::sleep_for(chrono::seconds(2));
        cout << endl;
    }

    getchar();
    return 0;
}
