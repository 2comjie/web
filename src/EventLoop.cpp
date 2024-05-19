#include "EventLoop.h"

#include "Channel.h"
#include "Epoll.h"
EventLoop::EventLoop() : ep(nullptr), quit(false) {
    ep = new Epoll();
}

EventLoop::~EventLoop() {
    if (ep != nullptr)
        delete ep;
}

#include <stdio.h>
void EventLoop::loop() {
    while (!quit) {
        std::vector<Channel*> chs = ep->poll(-1);
        for (auto ch : chs)
            ch->handleEvent();
    }
}

void EventLoop::updateChannel(Channel* ch) {
    ep->updateChannle(ch);
}