#ifndef MUDUO_NET_SOCKET_H
#define MUDUO_NET_SOCKET_H

#include <boost/noncopyable.hpp>

namespace duo {
    class InetAddress;

    // socket文件描述符封装，线程安全
    class Socket : boost::noncopyable {
    public:
        explicit Socket(int sockfd) : sockfd_(sockfd) {
        }

        ~Socket();

        int fd() const {
            return sockfd_;
        }

        // abort if address in use
        void bindAddress(const InetAddress& localaddr);

        // abort if address in use
        void listen();

        // 成功时，返回一个非负整数，套接字设置为非阻塞和close-on-exec。*peeraddr已分配。
        // 出错时，返回-1，而*peeraddr不变。
        int accept(InetAddress* peeraddr);

        // Enable/disable SO_REUSEADDR
        void setReuseAddr(bool on);

        void shutdownWrite();

        // Enable/disable TCP_NODELAY (disable/enable Nagle's algorithm).
        void setTcpNoDelay(bool on);

    private:
        const int sockfd_;
    };
}

#endif  // MUDUO_NET_SOCKET_H
