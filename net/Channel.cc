#include "Channel.h"
#include "EventLoop.h"
#include "../base/Logging.h"

#include <sstream>

#include <poll.h>

using namespace duo;

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(EventLoop* loop, int fdArg)
    : loop_(loop),
    fd_(fdArg),
    events_(0),
    revents_(0),
    index_(-1),
    eventHanding_(false) {
}

Channel::~Channel() {
    assert(!eventHanding_);
}

void Channel::update() {
    loop_->updateChannel(this);
}

void Channel::handleEvent() {
    eventHanding_ = true;
    if (revents_ & POLLNVAL) {
        // POLLNVAL表示套接字文件描述符未打开.关闭()它会是一个错误
        LOG_WARN << "Channel::handel_event() POLLNVAL";
    }

    if ((revents_ && POLLHUP) && !(revents_ & POLLIN)) {
        // POLLHUP表示套接字不再连接.在TCP中,这意味着FIN已被接收和发送.
        LOG_WARN << "Channel::handleEvent() POLLHUP";
        if (closeCallback_) closeCallback_();
    }
    if (revents_ & (POLLERR | POLLNVAL)) {
        if (errorCallback_) {
            errorCallback_();
        }
    }
    if (revents_ & (POLLIN | POLLPRI | POLLRDHUP)) {
        if (readCallback_) {
            readCallback_();
        }
    }
    if (revents_ & POLLOUT) {
        if (writeCallback_) {
            writeCallback_();
        }
    }
    eventHanding_ = false;
}
