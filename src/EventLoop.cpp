#include "EventLoop.h"

#include <string.h>

#include <vector>

#include "Channel.h"
#include "Epoll.h"
#include "ThreadPool.h"
EventLoop::EventLoop() : ep(nullptr), quit(false), threadPool(nullptr) {
    ep = new Epoll();
    threadPool = new ThreadPool();
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

void EventLoop::addThread(std::function<void()> func) {
    threadPool->add(func);
}