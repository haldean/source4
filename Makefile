BINARY=crender
CC=g++
CFLAGS=-c -Wall -g -O2 -I/usr/include/eigen3/
LDFLAGS=-lpng
SOURCES=$(wildcard src/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)

all: $(SOURCES) $(BINARY)
	
$(BINARY): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

run: all
	./${BINARY}

clean:
	rm -f ${OBJECTS} ${BINARY}
