#include "../EventLoop.h"
#include "../../base/Thread.h"
#include <stdio.h>

duo::EventLoop* g_loop;

void threadFunc() {
    g_loop->loop();
}

int main() {
    duo::EventLoop loop;
    g_loop = &loop;
    duo::Thread t(threadFunc);
    t.start();
    t.join();
}
