CC=gcc 
CFLAGS=-ansi -pedantic -Wall nbody.c

nbody: nbody.o

nbody.o: nbody.c
	$(CC) -c $(CFLAGS) 

clean:
	del nbody.exe nbody.o
# rm -rf nbody nbody.o