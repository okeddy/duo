#include "../EventLoop.h"

#include <boost/bind.hpp>
#include <stdio.h>

int cnt = 0;
duo::EventLoop* g_loop;

void printTid() {
    printf("pid = %d, tid = %d\n", getpid(), duo::CurrentThread::tid());
    printf("now %s\n", duo::Timestamp::now().toString().c_str());
}

void print(const char* msg) {
    printf("msg %s %s\n", duo::Timestamp::now().toString().c_str(), msg);
    if (++cnt == 20) {
        g_loop->quit();
    }
}

int main() {
    printTid();
    duo::EventLoop loop;
    g_loop = &loop;

    // print("main");
    // loop.runAfter(1, boost::bind(print, "once1"));
    // loop.runAfter(1.5, boost::bind(print, "once1.5"));
    // loop.runAfter(2.5, boost::bind(print, "once2.5"));
    // loop.runAfter(3.5, boost::bind(print, "once3.5"));
    // loop.runEvery(2, boost::bind(print, "every2"));
    // loop.runEvery(3, boost::bind(print, "every3"));

    loop.runEvery(1, boost::bind(print, "every1"));

    loop.loop();
    print("main loop exits");
    sleep(1);
}
