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

#ifndef DUO_BASE_THREADPOOL_H
#define DUO_BASE_THREADPOOL_H

#include "Condition.h"
#include "Mutex.h"
#include "Thread.h"

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include <deque>

namespace duo {
    class ThreadPool : boost::noncopyable
    {
    public:
        typedef boost::function<void ()> Task;

        explicit ThreadPool(const std::string& name = std::string());
        ~ThreadPool();

        void start(int numThreads);
        void stop();

        void run(const Task& f);

    private:
        void runInThread();
        Task take();

        MutexLock mutex_;
        Condition cond_;
        std::string name_;
        boost::ptr_vector<duo::Thread> threads_;
        std::deque<Task> queue_;
        bool running_;
    };
} // namespace duo


#endif  // DUO_BASE_THREADPOOL_H
