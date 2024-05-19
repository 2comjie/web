#include "EventLoop.h"

#include <string.h>

#include <vector>

#include "Channel.h"
#include "Epoll.h"
EventLoop::EventLoop() : ep(nullptr), quit(false) {
    ep = new Epoll();
}

EventLoop::~EventLoop() {
    delete ep;
}

void EventLoop::updateChannel(Channel* ch) {
    ep->updateChannel(ch);
}

void EventLoop::loop() {
    while (!quit) {
        std::vector<Channel*> chs = ep->poll(-1);
        for (auto ch : chs)
            ch->handleEvent();
    }
}