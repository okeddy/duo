/*
* Copyright (c) 2021, kd.
* All rights reserved.
*
* 文件名称：BlockingQueue.h
* 摘 要：
*
* 当前版本：1.0
* 作 者：kd
* 完成日期：2021年2月19日
*/

#ifndef DUO_BASE_BLOCKINGQUEUE_H
#define DUO_BASE_BLOCKINGQUEUE_H

#include "Condition.h"
#include "Mutex.h"

#include <boost/noncopyable.hpp>
#include <deque>
#include <assert.h>

namespace duo {
    template<typename T>
    class BlockingQueue : boost::noncopyable {
    public:
        BlockingQueue()
            : mutex_(),
            notEmpty_(mutex_),
            queue_() {
        }

        void put(const T& x) {
            MutexLockGuard lock(mutex_);
            queue_.push_back(x);
            notEmpty_.notify();
        }

        T take() {
            MutexLockGuard lock(mutex_);

            // always use a while-loop, due to spurious wakeup
            while (queue_.empty()) {
                notEmpty_.wait();
            }

            assert(!queue_.empty());
            T front(queue_.front());
            queue_.pop_front();
            return front;
        }

        size_t size() const {
            MutexLockGuard lock(mutex_);
            return queue_.size();
        }

    private:
        mutable MutexLock mutex_;
        Condition notEmpty_;
        std::deque<T> queue_;
    };
}

#endif  // DUO_BASE_BLOCKINGQUEUE_H
