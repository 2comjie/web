// // #include <arpa/inet.h>
// // #include <errno.h>
// // #include <fcntl.h>
// // #include <stdio.h>
// // #include <string.h>
// // #include <sys/epoll.h>
// // #include <sys/socket.h>
// // #include <unistd.h>

// // #include "util.h"

// // #define MAX_EVENTS 1024
// // #define READ_BUFFER 1024

// // void setnonblocking(int fd) {
// //     fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
// // }

// // int main() {
// //     int sockfd = socket(AF_INET, SOCK_STREAM, 0);
// //     errif(sockfd == -1, "socket create error");

// //     struct sockaddr_in serv_addr;
// //     bzero(&serv_addr, sizeof(serv_addr));
// //     serv_addr.sin_family = AF_INET;
// //     serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
// //     serv_addr.sin_port = htons(8888);

// //     errif(bind(sockfd, (sockaddr *)&serv_addr, sizeof(serv_addr)) == -1, "socket bind error");

// //     errif(listen(sockfd, SOMAXCONN) == -1, "socket listen error");

// //     int epfd = epoll_create1(0);
// //     errif(epfd == -1, "epoll create error");

// //     struct epoll_event events[MAX_EVENTS], ev;
// //     bzero(&events, sizeof(events));

// //     bzero(&ev, sizeof(ev));
// //     ev.data.fd = sockfd;
// //     ev.events = EPOLLIN | EPOLLET;
// //     setnonblocking(sockfd);
// //     epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);

// //     while (true) {
// //         int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
// //         errif(nfds == -1, "epoll wait error");
// //         for (int i = 0; i < nfds; ++i) {
// //             if (events[i].data.fd == sockfd) {  // 新客户端连接
// //                 struct sockaddr_in clnt_addr;
// //                 bzero(&clnt_addr, sizeof(clnt_addr));
// //                 socklen_t clnt_addr_len = sizeof(clnt_addr);

// //                 int clnt_sockfd = accept(sockfd, (sockaddr *)&clnt_addr, &clnt_addr_len);
// //                 errif(clnt_sockfd == -1, "socket accept error");
// //                 printf("new client fd %d! IP: %s Port: %d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));

// //                 bzero(&ev, sizeof(ev));
// //                 ev.data.fd = clnt_sockfd;
// //                 ev.events = EPOLLIN | EPOLLET;
// //                 setnonblocking(clnt_sockfd);
// //                 epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sockfd, &ev);
// //             } else if (events[i].events & EPOLLIN) {  // 可读事件
// //                 char buf[READ_BUFFER];
// //                 while (true) {
// //                     // 由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
// //                     bzero(&buf, sizeof(buf));
// //                     ssize_t bytes_read =
// //                         read(events[i].data.fd, buf, sizeof(buf));
// //                     if (bytes_read > 0) {
// //                         printf("message from client fd %d: %s\n", events[i].data.fd, buf);
// //                         write(events[i].data.fd, buf, sizeof(buf));
// //                     } else if (bytes_read == -1 && errno == EINTR) {  // 客户端正常中断、继续读取
// //                         printf("continue reading");
// //                         continue;
// //                     } else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {  // 非阻塞IO，这个条件表示数据全部读取完毕
// //                         printf("finish reading once, errno: %d\n", errno);
// //                         break;
// //                     } else if (bytes_read == 0) {  // EOF，客户端断开连接
// //                         printf("EOF, client fd %d disconnected\n", events[i].data.fd);
// //                         close(events[i].data.fd);  // 关闭socket会自动将文件描述符从epoll树上移除
// //                         break;
// //                     }
// //                 }
// //             } else {  // 其他事件，之后的版本实现
// //                 printf("something else happened\n");
// //             }
// //         }
// //     }
// //     close(sockfd);
// //     return 0;
// // }

// #include <errno.h>
// #include <fcntl.h>
// #include <stdio.h>
// #include <string.h>
// #include <unistd.h>

// #include <vector>

// #include "Epoll.h"
// #include "InetAddress.h"
// #include "Socket.h"
// #include "util.h"

// #define READ_BUFFER 1024

// void handleReadEvent(int);

// int main(void) {
//     Socket* serv_sock = new Socket();
//     InetAddress* serv_addr = new InetAddress("127.0.0.1", 8888);
//     serv_sock->bind(serv_addr);
//     serv_sock->listen();
//     Epoll* ep = new Epoll();
//     ep->addFd(serv_sock->getFd(), EPOLLIN);
//     while (true) {
//         std::vector<epoll_event> events = ep->poll();
//         int nfds = events.size();
//         for (int i = 0; i < nfds; ++i) {
//             if (events[i].data.fd == serv_sock->getFd()) {
//                 InetAddress* clnt_addr = new InetAddress();
//                 Socket* clnt_sock = new Socket(serv_sock->accept(clnt_addr));
//                 printf("new client fd %d! IP: %s Port: %d\n", clnt_sock->getFd(), inet_ntoa(clnt_addr->addr.sin_addr), ntohs(clnt_addr->addr.sin_port));
//             } else if (events[i].events & EPOLLIN) {
//                 handleReadEvent(events[i].data.fd);
//             } else {
//                 printf("somting else happened\n");
//             }
//         }
//     }
//     delete serv_sock;
//     delete serv_addr;
//     return 0;
// }

// void handleReadEvent(int sockfd) {
//     char buf[READ_BUFFER];
//     while (true) {
//         bzero(&buf, sizeof(buf));
//         ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
//         if (bytes_read > 0) {
//         } else if (bytes_read == -1 && errno == EINTR) {
//             printf("continue reading\n");
//             continue;
//         } else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {  // 非阻塞IO，这个条件表示数据全部读取完毕
//             printf("finish reading once, errno: %d\n", errno);
//             break;
//         } else if (bytes_read == 0) {
//             printf("EOF, client fd %d disconnected\n", sockfd);
//             close(sockfd);
//             break;
//         }
//     }
// }

// #include <errno.h>
// #include <fcntl.h>
// #include <stdio.h>
// #include <string.h>
// #include <unistd.h>

// #include <vector>

// #include "Epoll.h"
// #include "InetAddress.h"
// #include "Socket.h"
// #include "util.h"

// #define MAX_EVENTS 1024
// #define READ_BUFFER 1024

// void setnonblocking(int fd) {
//     fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
// }
// void handleReadEvent(int);

// int main() {
//     Socket *serv_sock = new Socket();
//     InetAddress *serv_addr = new InetAddress("127.0.0.1", 8888);
//     serv_sock->bind(serv_addr);
//     serv_sock->listen();
//     Epoll *ep = new Epoll();
//     serv_sock->setnonblocking();
//     ep->addFd(serv_sock->getFd(), EPOLLIN | EPOLLET);
//     while (true) {
//         std::vector<epoll_event> events = ep->poll();
//         int nfds = events.size();
//         for (int i = 0; i < nfds; ++i) {
//             if (events[i].data.fd == serv_sock->getFd()) {                     // 新客户端连接
//                 InetAddress *clnt_addr = new InetAddress();                    // 会发生内存泄露！没有delete
//                 Socket *clnt_sock = new Socket(serv_sock->accept(clnt_addr));  // 会发生内存泄露！没有delete
//                 printf("new client fd %d! IP: %s Port: %d\n", clnt_sock->getFd(), inet_ntoa(clnt_addr->addr.sin_addr), ntohs(clnt_addr->addr.sin_port));
//                 clnt_sock->setnonblocking();
//                 ep->addFd(clnt_sock->getFd(), EPOLLIN | EPOLLET);
//             } else if (events[i].events & EPOLLIN) {  // 可读事件
//                 handleReadEvent(events[i].data.fd);
//             } else {  // 其他事件，之后的版本实现
//                 printf("something else happened\n");
//             }
//         }
//     }
//     delete serv_sock;
//     delete serv_addr;
//     return 0;
// }

// void handleReadEvent(int sockfd) {
//     char buf[READ_BUFFER];
//     while (true) {  // 由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
//         bzero(&buf, sizeof(buf));
//         ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
//         if (bytes_read > 0) {
//             printf("message from client fd %d: %s\n", sockfd, buf);
//             write(sockfd, buf, sizeof(buf));
//         } else if (bytes_read == -1 && errno == EINTR) {  // 客户端正常中断、继续读取
//             printf("continue reading");
//             continue;
//         } else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {  // 非阻塞IO，这个条件表示数据全部读取完毕
//             printf("finish reading once, errno: %d\n", errno);
//             break;
//         } else if (bytes_read == 0) {  // EOF，客户端断开连接
//             printf("EOF, client fd %d disconnected\n", sockfd);
//             close(sockfd);  // 关闭socket会自动将文件描述符从epoll树上移除
//             break;
//         }
//     }
// }

#include "src/Server.h"

#include "src/EventLoop.h"

int main(void) {
    EventLoop* loop = new EventLoop();
    Server* server = new Server(loop);
    loop->loop();
    return 0;
}