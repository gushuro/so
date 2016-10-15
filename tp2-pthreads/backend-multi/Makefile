CC = g++
CFLAGS = -g -Wall -Werror -std=c++11
LDFLAGS = -pthread

BINARIES = backend

all: $(BINARIES)

backend: Backend_multi.cpp Backend_multi.h Enviar_recibir.o Enviar_recibir.h Encabezado.h Casillero.h RWLock.o
	$(CC) $(LDFLAGS) $(CFLAGS) Backend_multi.cpp Enviar_recibir.o RWLock.o -o backend

Enviar_recibir.o: Enviar_recibir.cpp Enviar_recibir.h Encabezado.h
	$(CC) $(CFLAGS) -c Enviar_recibir.cpp

RWLock.o: ../locks/RWLock.cpp ../locks/RWLock.h
	$(CC) $(CFLAGS) -c ../locks/RWLock.cpp

clean:
	rm -f *.o
	rm -f $(BINARIES)
