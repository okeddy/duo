#ifndef DUO_NET_CALLBACKS_H
#define DUO_NET_CALLBACKS_H

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#include "../base/Timestamp.h"

namespace duo {
    // All client visible callbacks go here.
    class Buffer;
    class TcpConnection;
    typedef boost::shared_ptr<TcpConnection> TcpConnectionPtr;

    typedef boost::function<void()> TimerCallback;
    typedef boost::function<void(const TcpConnectionPtr&)> ConnectionCallback;
    typedef boost::function<void(const TcpConnectionPtr&,
        Buffer* buf, Timestamp)> MessageCallback;
    typedef boost::function<void(const TcpConnectionPtr&) > WriteCompleteCallback;
    typedef boost::function<void(const TcpConnectionPtr&)> CloseCallback;
}

#endif  // DUO_NET_CALLBACKS_H
