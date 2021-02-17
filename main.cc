#include <iostream>
#include <vector>
#include "thread/Thread.h"
#include <boost/bind.hpp>

using namespace std;

void func(int num, const char* str) {
    for (size_t i = 0; i < num; i++)
    {
        cout << str << endl;
        sleep(1);
    }
}

int main() {
    duo::Thread t1(boost::bind(func, 3, "This is five!"), "Thread 1");
    t1.start();
    duo::Thread t2(boost::bind(func, 3, "This is two!"), "Thread 2");
    t2.start();
    t1.join();
    t2.join();

    return 0;
}
