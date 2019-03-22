CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g $(shell root-config --cflags)
LDFLAGS=-g $(shell root-config --ldflags)
LDLIBS=$(shell root-config --libs)

SRCS=main.cpp spreadsheet.ccp interpreter.cpp
OBJS=$(subst .ccp,.o,$(SRCS))

all: spreadsheet

spreadsheet: $(OBJS)
    $(CXX) $(LDFLAGS) -o spreadsheet $(OBJS) $(LDLIBS)

main.o: main.cpp

spreadshhet.o: spreadsheet.cpp

interpreter.o: interpreter.cpp

clean:
    $(RM) $(OBJS)

distclean: clean
    $(RM) tool
