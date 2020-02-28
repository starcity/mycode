CC=g++

OBJ=main
CXXFLAGS=-I./gtest/googletest/include/
CXXFLAGS+=-I./gtest/googlemock/include/
CXXFLAGS+=-L./gtest/lib/
CXXFLAGS+=-lgtest -lgmock -lpthread




CXXFLAGS+=-std=c++11


SOURCES=main.cpp

$(OBJ):$(SOURCES)
	$(CC) -o $@  $(SOURCES)  $(CXXFLAGS)
