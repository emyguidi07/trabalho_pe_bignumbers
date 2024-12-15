CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Wvla -g
OBJ = bignumber.o client.o

all: client

client: $(OBJ)
	$(CC) $(CFLAGS) -o cliente $(OBJ)

bignumber.o: bignumber.c bignumber.h
	$(CC) $(CFLAGS) -c bignumber.c

client.o: client.c bignumber.h
	$(CC) $(CFLAGS) -c client.c

clean:
	rm -f *.o client