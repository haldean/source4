BINARY=source4
CC=g++
CFLAGS=-c -Wall -g -O3 -I../include/ -I../include/png++/ -rdynamic
LDFLAGS=-lpng -L/usr/local/lib -lrt -rdynamic
SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)

all: $(SOURCES) $(BINARY)
	
$(BINARY): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

run: all
	./${BINARY} && feh test.png

clean:
	rm -f ${OBJECTS} ${BINARY}
