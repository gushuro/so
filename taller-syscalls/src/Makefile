CFLAGS= -Wall -O0 -g -ggdb -m32
NASMFLAGS= -g -ggdb
LDFLAGS=-static -m32 -L'/usr/lib32'
CC=gcc

BINS=launch ej1 ej2

OBJS=$(BINS:=.o)

all: $(BINS)

# Dependencias
launch.o: launch.c

ej1.o: ej1.c
ej2.o: ej2.c


clean:
	rm -rf $(OBJS) $(BINS)
