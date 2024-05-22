src:=src/Channel.cpp src/Epoll.cpp src/EventLoop.cpp src/Acceptor.cpp src/Buffer.cpp\
	src/InetAddress.cpp src/Server.cpp src/Socket.cpp src/Connection.cpp src/ThreadPool.cpp
.PHONY:all clean
all:serv clnt test
serv:$(src) serv.cpp
	g++ $^ -o $@ -I include -g -O3
clnt:clnt.cpp
	g++ clnt.cpp -o clnt -g -O3
test:$(src) test.cpp
	g++ $^ -o $@ -I include -g -O3
clean:
	-rm serv clnt test
