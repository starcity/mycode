#消息传递Makefile

CC=g++

SRCS=$(wildcard *.cpp */*.cpp)

OBJS=$(patsubst %.cpp, %.o, $(SRCS))

FLAG=-g

NAME=$(wildcard *.cpp)

#TARGET=$(patsubst %.cpp, %, $(NAME))

.PHONY:all clean

TARGET=message
all: $(TARGET)

$(TARGET):$(OBJS)
	$(CC) -o $@ $^ $(FLAG)

%.o:%.cpp %.h
	$(CC) -o  $@ -c $< -g

clean:
	rm -rf $(TARGET) $(OBJS)
