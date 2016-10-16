CC = g++
CFLAGS = -g -Wall -Werror -std=c++11
LDFLAGS = -pthread
BINARIES = test

%.o: %.cpp %.h
	$(CC) $(LDFLAGS) $(CFLAGS) -c $<

all: $(BINARIES)

test: RWLock.cpp RWLock.h test.cpp
	$(CC) $(LDFLAGS) $(CFLAGS) test.cpp RWLock.cpp -o test

clean:
	rm -f *.o
