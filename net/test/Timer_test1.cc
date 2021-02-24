#include "../EventLoop.h"
#include "../../base/Thread.h"
#include <stdio.h>

void threadFunc() {
    printf("threadFunc(): pid = %d, tid = %d\n",
        getpid(), duo::CurrentThread::tid());

    duo::EventLoop loop;
    loop.loop();
}

int main() {
    printf("main(): pid = %d, tid = %d\n",
        getpid(), duo::CurrentThread::tid());

    duo::EventLoop loop;

    duo::Thread thread(threadFunc);
    thread.start();

    loop.loop();
    pthread_exit(NULL);
}
