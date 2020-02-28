target=test
cc=g++

sources=http_client.cpp 
#CXXFLAGS=-I/root/github/spdlog/include
CXXFLAGS+=-std=c++11
#CXXFLAGS+=-lpthread
CXXFLAGS+=-levent


$(target):$(sources) 
	$(cc) -o $@ $(sources) $(CXXFLAGS)
