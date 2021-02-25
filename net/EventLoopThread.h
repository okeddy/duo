#ifndef DUO_NET_EVENTLOOPTHREAD_H
#define DUO_NET_EVENTLOOPTHREAD_H

#include "../base//Condition.h"
#include "../base//Mutex.h"
#include "../base//Thread.h"

#include <boost/noncopyable.hpp>

namespace duo {
    class EventLoop;

    class EventLoopThread : boost::noncopyable {
    public:
        EventLoopThread();
        ~EventLoopThread();
        EventLoop* startLoop();

    private:
        void threadFunc();

        EventLoop* loop_;
        bool exiting_;
        Thread thread_;
        MutexLock mutex_;
        Condition cond_;
    };
}

#endif  // DUO_NET_EVENTLOOPTHREAD_H
