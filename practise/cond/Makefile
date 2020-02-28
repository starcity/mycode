TARGET=cond

CURRENTPWD=$(shell pwd)
BASELIB=$(CURRENTPWD)
TOOLSPWD=$(CURRENTPWD)/../

CXXFLAGS += -DDEBUG
CXXFLAGS += -O0 -g -Wall
CXXFLAGS += -std=c++11
CXXFLAGS +=-I$(TOOLSPWD)

CC=g++

SOURCES=$(CURRENTPWD)/main.o  \
		$(CURRENTPWD)/cond.o  \

LIB=-lpthread 

$(TARGET):$(SOURCES)
	$(CC) -o $@ $^ $(CXXFLAGS) $(LIB) $(LIBDIR)


clean:
	rm $(SOURCES) $(TARGET) -f
