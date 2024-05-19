#pragma once
#include <sys/epoll.h>

#include <functional>
class EventLoop;
class Channel {
   private:
    EventLoop* loop;
    int fd;
    std::function<void()> callback;
    bool inEpoll;
    uint32_t events;
    uint32_t revents;

   public:
    Channel(EventLoop*, int);
    ~Channel();

    void handleEvent();

    void setEvents(uint32_t);
    void setRevents(uint32_t);
    void setInEpoll();
    void setCallback(std::function<void()>);

    uint32_t getEvents() const;
    uint32_t getRevents() const;
    bool getInEpoll() const;
    int getFd() const;

    void updateChannel();
};