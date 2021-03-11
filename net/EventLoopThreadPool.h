#ifndef DUO_NET_EVENTLOOPTHREADPOOL_H
#define DUO_NET_EVENTLOOPTHREADPOOL_H

#include "../base/Condition.h"
#include "../base/Mutex.h"
#include "../base/Thread.h"

#include <vector>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace duo {

    class EventLoop;
    class EventLoopThread;

    class EventLoopThreadPool : boost::noncopyable {
    public:
        EventLoopThreadPool(EventLoop* baseLoop);
        ~EventLoopThreadPool();
        void setThreadNum(int numThreads) { numThreads_ = numThreads; }
        void start();
        EventLoop* getNextLoop();

    private:
        EventLoop* baseLoop_;
        bool started_;
        int numThreads_;
        int next_;  // always in loop thread
        boost::ptr_vector<EventLoopThread> threads_;
        std::vector<EventLoop*> loops_;
    };
}

#endif  // DUO_NET_EVENTLOOPTHREADPOOL_H
