#ifndef DUO_NET_EVENTLOOP_H
#define DUO_NET_EVENTLOOP_H

#include "../base/Thread.h"

#include <boost/scoped_ptr.hpp>
#include <vector>

namespace duo {
    class Channel;
    class Poller;

    class EventLoop : boost::noncopyable {
    public:
        EventLoop();
        ~EventLoop();

        void loop();

        void quit();

        // internal use only
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

        typedef std::vector<Channel*> ChannelList;

        bool looping_;  // atomic
        bool quit_;
        const pid_t threadId_;
        boost::scoped_ptr<Poller> poller_;
        ChannelList activeChannels_;
    };
}

#endif  // DUO_NET_EVENTLOOP_H
