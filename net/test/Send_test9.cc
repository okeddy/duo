#include "../TcpServer.h"
#include "../EventLoop.h"
#include "../InetAddress.h"
#include <stdio.h>

void onConnection(const duo::TcpConnectionPtr& conn) {
    if (conn->connected()) {
        printf("onConnection(): new connection [%s] from %s\n",
            conn->name().c_str(),
            conn->peerAddress().toHostPort().c_str());
    } else {
        printf("onConnection(): connection [%s] is down\n",
            conn->name().c_str());
    }
}

void onMessage(const duo::TcpConnectionPtr& conn,
    duo::Buffer* buf,
    duo::Timestamp receiveTime) {
    printf("onMessage(): received %zd bytes from connection [%s] at %s\n",
        buf->readableBytes(),
        conn->name().c_str(),
        receiveTime.toFormattedString().c_str());

    conn->send(buf->retrieveAsString());
}

int main() {
    printf("main(): pid = %d\n", getpid());

    duo::InetAddress listenAddr("172.17.27.39", 7111);
    duo::EventLoop loop;

    duo::TcpServer server(&loop, listenAddr);
    server.setConnectionCallback(onConnection);
    server.setMessageCallback(onMessage);
    server.start();

    loop.loop();
}
