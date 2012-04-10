CC=gcc 
CFLAGS=-ansi -pedantic -Wall nbody.c

nbody: nbody.o
	$(CC) nbody.o -o nbody -lm

nbody.o: nbody.c
	$(CC) -c $(CFLAGS) 

clean:
	rm -rf nbody nbody.o # FOR LINUX
	#del nbody.exe nbody.o # FOR WINDOWS
	
