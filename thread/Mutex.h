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

#ifndef DUO_BASE_MUTEX_H
#define DUO_BASE_MUTEX_H

#include "Thread.h"

#include <boost/noncopyable.hpp>
#include <assert.h>
#include <pthread.h>

namespace duo {
    class MutexLock : boost::noncopyable
    {
    public:
        MutexLock() : holder_(0) {
            pthread_mutex_init(&mutex_, NULL);
        }

        ~MutexLock() {
            assert(holder_ == 0);
            pthread_mutex_destroy(&mutex_);
        }

        bool isLockedByThisThread() {
            return holder_ == CurrentThread::tid();
        }

        void assertLocked() {
            assert(isLockedByThisThread());
        }

        void lock() {
            pthread_mutex_lock(&mutex_);
            holder_ = CurrentThread::tid();
        }

        void unlock() {
            holder_ = 0;
            pthread_mutex_unlock(&mutex_);
        }

        pthread_mutex_t* getPthreadMutex() {
            return &mutex_;
        }

    private:
        pthread_mutex_t mutex_;
        pid_t holder_;
    };

    class MutexLockGuard : boost::noncopyable
    {
        public:
            explicit MutexLockGuard(MutexLock& mutex) : mutex_(mutex) {
                mutex_.lock();
            }

            ~MutexLockGuard() {
                mutex_.unlock();
            }
        
        private:
            MutexLock& mutex_;
    };
}

//防止遗漏变量名，产生一个临时对象又马上销毁了,结果没有锁住临界区。
#define MutexLockGuard(x) error "Missing guard object name"

#endif  // DUO_BASE_MUTEX_H
