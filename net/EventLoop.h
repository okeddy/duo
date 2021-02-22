#ifndef DUO_NET_EVENTLOOP_H
#define DUO_NET_EVENTLOOP_H

#include "../base/Thread.h"

namespace duo {
    class EventLoop : boost::noncopyable {
    public:
        EventLoop();
        ~EventLoop();

        void loop();

        void assertInLoopThread() {
            if (isInLoopThread()) {
                abortNotInLoopThread();
            }
        }

        bool isInLoopThread() const {
            return threadId_ == CurrentThread::tid();
        }

    private:
        void abortNotInLoopThread();

        bool looping_;  // atomic
        const pid_t threadId_;
    };
}

#endif  // DUO_NET_EVENTLOOP_H
