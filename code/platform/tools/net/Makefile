CXXFLAGS += -O0 -g -Wall
CXXFLAGS += -std=c++11
CC=g++


#LIB=



TCP_SERVER=tcp_server.cpp
TCP_CLIENT=tcp_client.cpp
UDP_SERVER=udp_server.cpp
UDP_CLIENT=udp_client.cpp


.PHONY :all
all:tcp_server tcp_client udp_server udp_client


tcp_server:$(TCP_SERVER)
	echo "make tcp_server"
	$(CC) -c $^ $(CXXFLAGS) $(LIB)
	ar crv lib$@.a $@.o
	$(CC) -fPIC -shared -o lib$@.so $^ $(CXXFLAGS) $(LIB)
	cp lib$@.so lib$@.a ../lib/ -f

tcp_client:$(TCP_CLIENT)
	echo "make tcp_client"
	$(CC) -c  $^ $(CXXFLAGS) $(LIB)
	ar crv lib$@.a $@.o
	$(CC)  -fPIC -shared -o lib$@.so $^ $(CXXFLAGS) $(LIB)
	cp lib$@.so lib$@.a ../lib/ -f

udp_server:$(UDP_SERVER)
	echo "make udp_server"
	$(CC) -c  $^ $(CXXFLAGS) $(LIB)
	ar crv lib$@.a $@.o
	$(CC)  -fPIC -shared -o lib$@.so $^ $(CXXFLAGS) $(LIB)
	cp lib$@.so lib$@.a ../lib/ -f

udp_client:$(UDP_CLIENT)
	echo "make udp_client"
	$(CC) -c  $^ $(CXXFLAGS) $(LIB)
	ar crv lib$@.a $@.o
	$(CC)  -fPIC -shared -o lib$@.so $^ $(CXXFLAGS) $(LIB)
	cp lib$@.so lib$@.a ../lib/ -f



