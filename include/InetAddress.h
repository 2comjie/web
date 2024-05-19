#pragma once

#include <arpa/inet.h>
struct InetAddress {
    sockaddr_in addr;
    socklen_t addrLen;

    InetAddress(const char*, int);
    InetAddress();
    ~InetAddress();
};