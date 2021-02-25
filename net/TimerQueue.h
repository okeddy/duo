#ifndef DUO_NET_TIMERQUEUE_H
#define DUO_NET_TIMERQUEUE_H

#include <set>
#include <vector>

#include <boost/noncopyable.hpp>

#include "../base/Timestamp.h"
#include "../base/Mutex.h"
#include "Callbacks.h"
#include "Channel.h"

namespace duo {
    class EventLoop;
    class Timer;
    class TimerId;

    class TimerQueue : boost::noncopyable {
    public:
        TimerQueue(EventLoop* loop);
        ~TimerQueue();

        // 线程安全，通常从其他线程调用
        // 如果interval > 0.0则定时器重复
        TimerId addTimer(const TimerCallback& cb,
            Timestamp when, double interval);

        void cancel(TimerId timerId);

    private:
        typedef std::pair<Timestamp, Timer*> Entry;
        typedef std::set<Entry> TimerList;

        void addTimerInLoop(Timer* tiemr);

        // 当 timerfd 可读时调用
        void handleRead();

        // 移出所有到期计时器
        std::vector<Entry> getExpired(Timestamp now);
        void reset(const std::vector<Entry>& expired, Timestamp now);

        bool insert(Timer* timer);

        EventLoop* loop_;
        const int timerfd_;
        Channel timerfdChannel_;

        // 计时器列表按到期时间排序
        TimerList timers_;
    };
}

#endif // DUO_NET_TIMERQUEUE_H
