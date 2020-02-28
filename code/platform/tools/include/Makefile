CXXFLAGS += -O0 -g -Wall
CXXFLAGS += -std=c++11
CXXFLAGS += -L../lib/
CC=g++



TCP_SERVER=test_tcp_server.cpp
TCP_CLIENT=test_tcp_client.cpp
UDP_SERVER=test_udp_server.cpp
UDP_CLIENT=test_udp_client.cpp


.PHONY :all
all:tcp_server tcp_client udp_server udp_client


tcp_server:$(TCP_SERVER)
	$(CC) -o $@ $^ $(CXXFLAGS) -l$@

tcp_client:$(TCP_CLIENT)
	$(CC)  -o $@ $^ $(CXXFLAGS) -l$@ -lpthread

udp_server:$(UDP_SERVER)
	$(CC)  -o $@ $^ $(CXXFLAGS) -l$@

udp_client:$(UDP_CLIENT)
	$(CC)  -o $@ $^ $(CXXFLAGS) -l$@



clean:
	rm  tcp_server tcp_client udp_server udp_client -f
