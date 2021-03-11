#include "../TcpServer.h"
#include "../EventLoop.h"
#include "../InetAddress.h"
#include <stdio.h>

std::string message1;
std::string message2;

void onConnection(const duo::TcpConnectionPtr& conn) {
    if (conn->connected()) {
        printf("onConnection(): new connection [%s] from %s\n",
            conn->name().c_str(),
            conn->peerAddress().toHostPort().c_str());
        conn->send(message1);
        conn->send(message2);
        conn->shutdown();
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

    buf->retrieveAll();
}

int main(int argc, char* argv[]) {
    printf("main(): pid = %d\n", getpid());

    int len1 = 100;
    int len2 = 200;

    if (argc > 2) {
        len1 = atoi(argv[1]);
        len2 = atoi(argv[2]);
    }

    message1.resize(len1);
    message2.resize(len2);
    std::fill(message1.begin(), message1.end(), 'A');
    std::fill(message2.begin(), message2.end(), 'B');

    duo::InetAddress listenAddr("172.17.27.39", 7111);
    duo::EventLoop loop;

    duo::TcpServer server(&loop, listenAddr);
    server.setConnectionCallback(onConnection);
    server.setMessageCallback(onMessage);
    server.start();

    loop.loop();
}
