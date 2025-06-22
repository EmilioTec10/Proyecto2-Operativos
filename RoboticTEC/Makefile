CC = gcc
AR = ar
CFLAGS = -Wall -O2

all: biblioteca.a

biblioteca.a: biblioteca.o
	$(AR) rcs biblioteca.a biblioteca.o

biblioteca.o: biblioteca.c biblioteca.h
	$(CC) $(CFLAGS) -c biblioteca.c

clean:
	rm -f *.o *.a
