#ifndef DUO_NET_ACCEPTOR_H
#define DUO_NET_ACCEPTOR_H

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

#include "Channel.h"
#include "Socket.h"

namespace duo {
    class EventLoop;
    class InetAddress;

    class Acceptor : boost::noncopyable {
    public:
        typedef boost::function<void(int sockfd,
            const InetAddress&)> NewConnectionCallback;

        Acceptor(EventLoop* loop, const InetAddress& listenAddr);

        void setNewConnectionCallback(const NewConnectionCallback& cb) {
            newConnectionCallback_ = cb;
        }

        bool listenning() const {
            return listenning_;
        }
        void listen();

    private:
        void handleRead();

        EventLoop* loop_;
        Socket acceptSocket_;
        Channel acceptChannel_;
        NewConnectionCallback newConnectionCallback_;
        bool listenning_;
    };
}

#endif  // DUO_NET_ACCEPTOR_H
