#CC = gcc-4.8
CXX = g++
AR = ar rcs
RM = -rm -rf

all : myplistcpp

INCLUDE = -I./src -I./include

CXXFLAGS = -Wall -O -fPIC

SRCS = $(wildcard src/*.cpp)
OBJS = $(patsubst %.cpp,%.o, $(SRCS))

%.o:%.cpp
	$(CXX) -c $(CXXFLAGS) $(INCLUDE)  $< -o $@

myplistcpp : $(OBJS)
	$(CXX) -shared $(OBJS) -o libplistcpp.so

clean:
	$(RM) src/*.o
