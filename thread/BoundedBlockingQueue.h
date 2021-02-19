/*
* Copyright (c) 2021, kd.
* All rights reserved.
*
* 文件名称：filename.h
* 摘 要：
*
* 当前版本：1.0
* 作 者：kd
* 完成日期：2021年2月19日
*/

#ifndef DUO_BASE_BOUNDEDBLOCKINGQUEUE_H
#define DUO_BASE_BOUNDEDBLOCKINGQUEUE_H

#include "Condition.h"
#include "Mutex.h"

#include <boost/noncopyable.hpp>
#include <boost/circular_buffer.hpp>
#include <assert.h>

namespace duo {
    template<typename T>
    class BoundedBlockingQueue : boost::noncopyable
    {
    public:
        explicit BoundedBlockingQueue(int max_size)
            :   mutex_(),
                notFull_(mutex_),
                notEmpty_(mutex_),
                queue_(max_size)
        { }

        void put(const T& x) {
            MutexLockGuard lock(mutex_);

            while (queue_.full())
            {
                notFull_.wait();
            }
            
            assert(!queue_.full());
            queue_.push_back(x);
            notEmpty_.notify();
        }

        T take() {
            MutexLockGuard lock(mutex_);

            while (queue_.empty())
            {
                notEmpty_.wait();
            }
            
            assert(!queue_.empty());
            T front(queue_.front());
            queue_.pop_front();
            notFull_.notify();
            return front;
        }

        bool empty() const {
            MutexLockGuard lock(mutex_);
            return queue_.empty();
        }

        bool full() const {
            MutexLockGuard lock(mutex_);
            return queue_.full();
        }

        size_t size() const {
            MutexLockGuard lock(mutex_);
            return queue_.size();
        }

        size_t capacity() const {
            MutexLockGuard lock(mutex_);
            return queue_.capacity();
        }

    private:
        mutable MutexLock mutex_;
        Condition notFull_;
        Condition notEmpty_;
        boost::circular_buffer<T> queue_;
    };
}

#endif  // DUO_BASE_BOUNDEDBLOCKINGQUEUE_H
