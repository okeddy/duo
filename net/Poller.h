#ifndef DUO_NET_POLLER_H
#define DUO_NET_POLLER_H

#include <map>
#include <vector>

#include "../base/Timestamp.h"
#include "EventLoop.h"

struct pollfd;

namespace duo {
    class Channel;

    class Poller : boost::noncopyable {
    public:
        typedef std::vector<Channel*> ChannelList;

        Poller(EventLoop* loop);
        ~Poller();

        // Polls the I/O events.
        // Must be called in the loop thread.
        Timestamp poll(int timeoutMs, ChannelList* activeChannels);

        // Changes the interested I/O events.
        // Must be called in the loop thread.
        void updateChannel(Channel* channels);

        void assertInLoopThread() {
            ownerLoop_->assertInLoopThread();
        }

    private:
        void fillActiveChannels(int numEvents,
            ChannelList* activeChannels) const;

        typedef std::vector<struct pollfd> PollFdList;
        typedef std::map<int, Channel*> ChannelMap;

        EventLoop* ownerLoop_;
        PollFdList pollfds_;
        ChannelMap channels_;
    };
}

#endif  // DUO_NET_POLLER_H
