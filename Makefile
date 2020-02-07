all: gameoflife.o libgol.so
	gcc -Wall -Wextra -pedantic -std=c11 -o main gameoflife.o -llibgol.so

gameoflife.o: gameoflife.c
	gcc -Wall -Wextra -pedantic -std=c11 -c -o gameoflife.o gameoflife.c

gol.o: gol.h
	gcc -Wall -Wextra -pedantic -std=c11 -c -fPIC gol.h -o gol.o

libgol.so: gol.o
	gcc -Wall -Wextra -pedantic -std=c11 gol.o -shared -o libgol.so

clean:
	rm main gol.o gameoflife.o libgol.so
