#include "../Acceptor.h"
#include "../EventLoop.h"
#include "../InetAddress.h"
#include "../SocketsOps.h"
#include <stdio.h>

using namespace duo;

void newConnection(int sockfd, const ::InetAddress& peerAddr) {
    printf("newConnection(): accepted a new connection from %s\n",
        peerAddr.toHostPort().c_str());
    ::write(sockfd, "How are you?\n", 13);
    ::sockets::close(sockfd);
}

int main() {
    printf("main(): pid = %d\n", getpid());

    ::InetAddress listenAddr("172.17.27.39", 7111);
    ::EventLoop loop;

    ::Acceptor acceptor(&loop, listenAddr);
    acceptor.setNewConnectionCallback(newConnection);
    acceptor.listen();

    loop.loop();
}
