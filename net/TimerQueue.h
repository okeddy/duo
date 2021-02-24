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

        TimerId addTimer(const TimerCallback& cd,
            Timestamp when, double interval);

        void cancel(TimerId timerId);

    private:
        typedef std::pair<Timestamp, Timer*> Entry;
        typedef std::set<Entry> TimerList;

        void handleRead();

        std::vector<Entry> getExpired(Timestamp now);
        void reset(const std::vector<Entry>& expired, Timestamp now);

        bool insert(Timer* timer);

        EventLoop* loop_;
        const int timerfd_;
        Channel timerfdChannel_;

        TimerList timers_;
    };
}

#endif // DUO_NET_TIMERQUEUE_H
