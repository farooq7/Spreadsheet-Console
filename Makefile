CC=gcc
CXX=g++
RM=rm -f

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
