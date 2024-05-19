#include "Channel.h"

#include "EventLoop.h"
Channel::Channel(int _fd, EventLoop* _loop) : fd(_fd), loop(_loop), inEpoll(false), callback(nullptr), events(0), revents(0) {
}

Channel::~Channel() {
}

void Channel::handleEvent() {
    callback();
}

uint32_t Channel::getEvents() {
    return events;
}

uint32_t Channel::getRevents() {
    return revents;
}

int Channel::getFd() {
    return fd;
}

bool Channel::getInEpoll() {
    return inEpoll;
}

void Channel::setEvents(uint32_t ev) {
    events = ev;
}

void Channel::setRevents(uint32_t ev) {
    revents = ev;
}

void Channel::setInEpoll() {
    inEpoll = true;
}

void Channel::setCallback(std::function<void()> cb) {
    callback = cb;
}

void Channel::updateChannel() {
    loop->updateChannel(this);
}