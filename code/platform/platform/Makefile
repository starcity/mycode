#!make


TARGET=mdsServer

CURRENTPWD=$(shell pwd)
BASELIB=$(CURRENTPWD)
TOOLSPWD=$(CURRENTPWD)/../tools

CXXFLAGS += -DDEBUG
CXXFLAGS += -O0 -g -Wall
CXXFLAGS += -std=c++11
CXXFLAGS +=-I$(TOOLSPWD)

CC=g++

SOURCES=$(CURRENTPWD)/main.o                      \
      	$(TOOLSPWD)/log/Log.o                   \
        $(CURRENTPWD)/httpserver.o                \
        $(CURRENTPWD)/businessmanagement.o        \
	$(CURRENTPWD)/cencode.o                      \
        $(TOOLSPWD)/json/json_reader.o          \
        $(TOOLSPWD)/json/json_value.o           \
        $(TOOLSPWD)/json/json_writer.o
#	$(TOOLSPWD)/tinyxml/CTcxml.o			\
#       $(TOOLSPWD)/tinyxml/tinyxmlerror.o				\
#       $(TOOLSPWD)/tinyxml/tinyxmlparser.o				\
#       $(TOOLSPWD)/tinyxml/tinyxml.o					\
#       $(TOOLSPWD)/tinyxml/tinystr.o				\

LIBDIR=-L.
LIB=-lpthread -levent -lyaml -Wl,-rpath,./ 

$(TARGET):$(SOURCES)
	$(CC) -o $@ $^ $(CXXFLAGS) $(LIB) $(LIBDIR)


clean:
	rm $(SOURCES) $(TARGET) -f

