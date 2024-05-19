src:=src/Channel.cpp src/Epoll.cpp src/EventLoop.cpp src/Acceptor.cpp\
	src/InetAddress.cpp src/Server.cpp src/Socket.cpp src/Connection.cpp serv.cpp
.PHONY:all clean
all:serv clnt
serv:$(src)
	g++ $^ -o $@ -I include -g
clnt:clnt.cpp
	g++ clnt.cpp -o clnt -g
clean:
	-rm serv clnt
