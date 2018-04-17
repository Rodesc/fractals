
# TODO
all: main.o

fractal_test1: fractal.o main.o
	gcc -o fractal_test1 fractal.o main.o

main.o: main.c libfractal/fractal.h lib
	gcc -o main.o -c main.c -W -Wall

lib:
	@(cd libfractal && $(MAKE))
