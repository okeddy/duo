/*
* Copyright (c) 2021, kd.
* All rights reserved.
*
* 文件名称：main.cc
* 摘 要：
*
* 当前版本：1.0
* 作 者：kd
* 完成日期：2021年2月19日
*/

#include "base/ThreadPool.h"
#include "base/CountDownLatch.h"

#include <boost/bind.hpp>
#include <stdio.h>

void print() {
    printf("tid=%d\n", duo::CurrentThread::tid());
}

void printString(const std::string& str) {
    printf("tid=%d, str=%s\n", duo::CurrentThread::tid(), str.c_str());
}

int main() {
    duo::ThreadPool pool("MainThreadPool");
    pool.start(5);

    pool.run(print);
    pool.run(print);
    for (int i = 0; i < 100; ++i) {
        char buf[32];
        snprintf(buf, sizeof buf, "task %d", i);
        pool.run(boost::bind(printString, std::string(buf)));
    }

    duo::CountDownLatch latch(1);
    pool.run(boost::bind(&duo::CountDownLatch::countDown, &latch));
    latch.wait();
    pool.stop();
}
