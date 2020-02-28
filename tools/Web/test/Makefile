CLIENT=client
SERVER=server

CXXFLAGS=-std=c++11 -g
CXXFLAGS+=-I../
CXXFLAGS+=-I/root/code/code/libevent-2.1.11-stable/include
CXXFLAGS+=-I../common/
CXXFLAGS+=-I../client/
CXXFLAGS+=-I../server/
CXXFLAGS+=-L/root/code/code/libevent-2.1.11-stable/.libs/
CXXFLAGS+=-Wl,-dn -levent -levent_openssl 
CXXFLAGS+=-Wl,-dy -lkrb5 -lpthread -lcrypto -lssl
CSOURCES=client_test.cpp \
	../common/hostcheck.cpp \
	../common/openssl_hostname_validation.cpp \
	../client/url_client.cpp	\


SSOURCES=server_test.cpp  \
		 ../server/web_server.cpp   \
		 ../common/hostcheck.cpp \
	     ../common/openssl_hostname_validation.cpp \

CC=g++

all:$(CLIENT) $(SERVER)

$(CLIENT):$(CSOURCES)
	$(CC) -o $@ $^ $(CXXFLAGS)

$(SERVER):$(SSOURCES)
	$(CC) -o $@ $^ $(CXXFLAGS)


clean:
	rm $(CLIENT) $(SERVER)  -f
