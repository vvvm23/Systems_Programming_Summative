all: gameoflife.o libgol.so
	gcc -Wall -Wextra -pedantic -std=c11 -g -o gameoflife ./libgol.so gameoflife.o

gameoflife.o: gameoflife.c
	gcc -Wall -Wextra -pedantic -std=c11 -g -c -o gameoflife.o gameoflife.c

gol.o: gol.h
	gcc -Wall -Wextra -pedantic -std=c11 -g -c -fPIC gol.c -o gol.o

libgol.so: gol.o
	gcc -Wall -Wextra -pedantic -std=c11 -g gol.o -shared -o libgol.so

clean:
	rm gameoflife gol.o gameoflife.o libgol.so
