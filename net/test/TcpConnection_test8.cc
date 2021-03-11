#include "../TcpServer.h"
#include "../EventLoop.h"
#include "../InetAddress.h"
#include <stdio.h>

#include "../../base/Logging.h"
#include "../../base/LogFile.h"

void onConnection(const duo::TcpConnectionPtr& conn) {
    if (conn->connected()) {
        LOG_INFO << "onConnection(): new connection [" << conn->name().c_str()
            << "] from " << conn->peerAddress().toHostPort().c_str();
    } else {
        LOG_INFO << "onConnection(): connection [" << conn->name().c_str()
            << "] is down";
    }
}

void onMessage(const duo::TcpConnectionPtr& conn,
    duo::Buffer* buf,
    duo::Timestamp receiveTime) {
    LOG_INFO << "onMessage(): received " << buf->readableBytes()
        << " bytes from connection [" << conn->name().c_str()
        << "] at " << receiveTime.toFormattedString().c_str();
    LOG_INFO << "onMessage(): [" << buf->retrieveAsString().c_str()
        << "]";
}

boost::scoped_ptr<duo::LogFile> g_logFile;

void outputFunc(const char* msg, int len) {
    g_logFile->append(msg, len);
}

void flushFunc() {
    g_logFile->flush();
}

int main(int argc, char* argv[]) {
    char name[256];
    strncpy(name, argv[0], 256);
    g_logFile.reset(new duo::LogFile(::basename(name), 200 * 1000));
    duo::Logger::setOutput(outputFunc);
    duo::Logger::setFlush(flushFunc);

    printf("main(): pid = %d\n", getpid());

    duo::InetAddress listenAddr("172.17.27.39", 7111);
    duo::EventLoop loop;

    duo::TcpServer server(&loop, listenAddr);
    server.setConnectionCallback(onConnection);
    server.setMessageCallback(onMessage);
    if (argc > 1) {
        server.setThreadNum(atoi(argv[1]));
    }
    server.start();

    loop.loop();
}
