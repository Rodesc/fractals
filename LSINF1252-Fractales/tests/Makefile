all: test1
	./test1

test1: test1.o ../libfractal/tools.o ../libfractal/fractal.o
	gcc -Wall -lcunit -o test1 ../libfractal/fractal.o test1.o ../libfractal/tools.o -lSDL -L$(HOME)/local/lib -lpthread
	
test1.o: test1.c
	gcc -Wall -c test1.c -I$(HOME)/local/include
