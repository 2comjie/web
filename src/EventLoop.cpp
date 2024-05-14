#include "EventLoop.h"

#include <vector>

#include "Channel.h"
#include "Epoll.h"
#include "EventLoop.h"
EventLoop::EventLoop() : ep(nullptr), quit(false) {
    ep = new Epoll();
}

EventLoop::~EventLoop() {
    delete ep;
}

void EventLoop::loop() {
    while (!quit) {
        std::vector<Channel*> activeChannels = ep->poll(-1);
        for (auto it = activeChannels.begin(); it != activeChannels.end(); it++)
            (*it)->handleEvent();
    }
}

void EventLoop::updateChannel(Channel* ch) {
    ep->updateChannel(ch);
}