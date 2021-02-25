#ifndef DUO_NET_EVENTLOOP_H
#define DUO_NET_EVENTLOOP_H

#include "../base/Timestamp.h"
#include "../base/Thread.h"
#include "../base/Mutex.h"
#include "Callbacks.h"
#include "TimerId.h"

#include <boost/scoped_ptr.hpp>
#include <vector>

namespace duo {
    class Channel;
    class Poller;
    class TimerQueue;

    class EventLoop : boost::noncopyable {
    public:
        typedef boost::function<void()> Functor;

        EventLoop();
        ~EventLoop();

        void loop();

        void quit();

        // Runs callback immediately in the loop thread.
        // It wakes up the loop, and run the cb.
        // If in the same loop thread, cb is run within the function.
        // Safe to call from other threads.
        void runInLoop(const Functor& cb);
        // Queues callback in the loop thread.
        // Runs after finish pooling.
        // Safe to call from other threads.
        void queueInLoop(const Functor& cb);

        // Time when poll returns, usually means data arrivial.
        Timestamp pollReturnTime() const { return pollReturnTime_; }

        // timer
        // Runs callback at "time"
        // Safe to call from other threads.
        TimerId runAt(const Timestamp& time, const TimerCallback& cb);

        // Runs callback after @c delay seconds.
        // Safe to call from other threads.
        TimerId runAfter(double delay, const TimerCallback& cb);

        // Runs callback every @c interval seconds.
        // Safe to call from other threads.
        TimerId runEvery(double interval, const TimerCallback& cb);

        void cancel(TimerId timerId);

        // internal use only
        void wakeup();
        void updateChannel(Channel* channel);

        void assertInLoopThread() {
            if (!isInLoopThread()) {
                abortNotInLoopThread();
            }
        }

        bool isInLoopThread() const {
            return threadId_ == CurrentThread::tid();
        }

    private:
        void abortNotInLoopThread();
        void handleRead();  // wake up
        void doPendingFunctors();

        typedef std::vector<Channel*> ChannelList;

        bool looping_;  // atomic
        bool quit_;
        bool callingPendingFunctors_;   // atomic
        const pid_t threadId_;
        Timestamp pollReturnTime_;
        boost::scoped_ptr<Poller> poller_;
        boost::scoped_ptr<TimerQueue> timerQueue_;
        int wakeupFd_;
        // 不像定时器队列在内部一样，我们不希望暴露内部通道给外部
        boost::scoped_ptr<Channel> wakeupChannel_;
        ChannelList activeChannels_;
        MutexLock mutex_;

        // @GuardedBy mutex_
        std::vector<Functor> pendingFunctors_;
    };
}

#endif  // DUO_NET_EVENTLOOP_H
