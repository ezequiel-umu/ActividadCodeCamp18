CC=g++
CFLAGS=-O3 -funroll-loops -c -std=c++11
LDFLAGS=-O2 -lm
SOURCES=engine/Bot.cc engine/State.cc algos/floydwarshall.cc main.cc
OBJECTS=$(addsuffix .o, $(basename ${SOURCES}))
EXECUTABLE=main

#Uncomment the following to enable debugging
#CFLAGS+=-g -DDEBUG

all: $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cc.o: *.h
	$(CC) $(CFLAGS) $< -o $@

clean: 
	-rm -f ${EXECUTABLE} ${OBJECTS} *.d
	-rm -f debug.txt

.PHONY: all clean

