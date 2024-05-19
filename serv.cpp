#include "EventLoop.h"
#include "Server.h"

int main(void) {
    EventLoop* loop = new EventLoop();
    Server* serv = new Server(loop);
    loop->loop();
    return 0;
}