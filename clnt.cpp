#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define BUFFER_READ 1024
int main(void) {
    int servSock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in servAddr;
    bzero(&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servAddr.sin_port = htons(8888);

    int cnt = connect(servSock, (sockaddr*)&servAddr, sizeof(servAddr));
    if (cnt == -1)
        printf("hi");

    char buf[BUFFER_READ];
    while (true) {
        printf("Input: ");
        bzero(buf, sizeof(buf));
        fgets(buf, sizeof(buf), stdin);
        if (!strcmp(buf, "q\n") || !strcmp(buf, "Q\n"))
            break;
        write(servSock, buf, sizeof(buf));
        bzero(buf, sizeof(buf));
        ssize_t len = read(servSock, buf, sizeof(buf));
        if (len < 0) {
            printf("disconnect\n");
            break;
        } else {
            printf("Serv: %s", buf);
        }
    }
    close(servSock);
    return 0;
}