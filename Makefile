CC=gcc 
CFLAGS=-ansi -pedantic -Wall nbody.c

nbody: nbody.o

nbody.o: nbody.c
	$(CC) $(CFLAGS) -c nbody.c

clean:
	del nbody.exe nbody.o
# rm -rf nbody nbody.o