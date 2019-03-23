CC=gcc
CXX=g++
RM=rm -f

CPPFLAGS=-g $(shell root-config --cflags)
LDFLAGS=-g $(shell root-config --ldflags)
LDLIBS=$(shell root-config --libs)

SRCS=main.cpp spreadsheet.cpp interpreter.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: spreadsheet

spreadsheet: $(OBJS)
	$(CXX) -Wall -o spreadsheet $(OBJS)

main.o: main.cpp

spreadsheet.o: spreadsheet.cpp

interpreter.o: interpreter.cpp

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) spreadsheet
