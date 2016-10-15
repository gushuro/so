CC = g++
CFLAGS = -g -Wall -Werror -std=c++11
LDFLAGS = -pthread
BINARIES = RWLockTest

%.o: %.cpp %.h
	$(CC) $(LDFLAGS) $(CFLAGS) -c $<

all: $(BINARIES)

test: RWLock.cpp RWLock.h test.cpp
	$(CC) $(LDFLAGS) $(CFLAGS) test.cpp RWLock.cpp -o test


RWLockTest: RWLock.cpp RWLock.h RWLockTest.cpp
	$(CC) $(LDFLAGS) $(CFLAGS) RWLockTest.cpp RWLock.cpp -o RWLockTest


clean:
	rm -f *.o
