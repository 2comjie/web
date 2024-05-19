#include "Channel.h"

#include <unistd.h>

#include "EventLoop.h"
Channel::Channel(EventLoop* _loop, int _fd) : loop(_loop), fd(_fd), callback(nullptr), inEpoll(false), events(0), revents(0) {
}

Channel::~Channel() {
}

void Channel::handleEvent() {
    callback();
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

void Channel::setCallback(std::function<void()> _cb) {
    callback = _cb;
}

uint32_t Channel::getEvents() const {
    return events;
}

uint32_t Channel::getRevents() const {
    return revents;
}

bool Channel::getInEpoll() const {
    return inEpoll;
}

int Channel::getFd() const {
    return fd;
}

void Channel::updateChannel() {
    loop->updateChannel(this);
}