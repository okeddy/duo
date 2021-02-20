/*
* Copyright (c) 2021, kd.
* All rights reserved.
*
* 文件名称：CountDownLatch.h
* 摘 要：
*
* 当前版本：1.0
* 作 者：kd
* 完成日期：2021年2月19日
*/

#ifndef DUO_BASE_COUNTDOWNLATCH_H
#define DUO_BASE_COUNTDOWNLATCH_H

#include "Mutex.h"
#include "Condition.h"

#include <boost/noncopyable.hpp>

namespace duo {
    class CountDownLatch : boost::noncopyable {
    public:
        explicit CountDownLatch(int count)
            : mutex_(),
            condition_(mutex_),
            count_(count) {
        }

        void wait() {
            MutexLockGuard lock(mutex_);

            while (count_ > 0) {
                condition_.wait();
            }
        }

        void countDown() {
            MutexLockGuard lock(mutex_);
            --count_;
            if (!count_) {
                condition_.notifyAll();
            }
        }

        int getCount() const {
            MutexLockGuard lock(mutex_);
            return count_;
        }

    private:
        mutable MutexLock mutex_;
        Condition condition_;
        int count_;
    };
}

#endif  // DUO_BASE_COUNTDOWNLATCH_H
