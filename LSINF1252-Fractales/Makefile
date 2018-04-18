
# TODO
#all: main.o
#
#fractal_test1: fractal.o main.o
#	gcc -o fractal_test1 fractal.o main.o

#main.o: main.c libfractal/fractal.h lib
#	gcc -o main.o -c main.c -W -Wall

#lib:
#	@(cd libfractal && $(MAKE))
#
all: main.o fractal_test1
	echo ''all : ok''

fractal_test1: main.o lib
	gcc -std=c99 -o fractal_test1 main.o -g -Wall -W
	echo ''fractal_test1 : ok''

main.o: main.c libfractal/fractal.h lib
	gcc -std=c99 -c main.c -g -Wall -W
	echo ''main.o : ok''

lib:
	@(cd libfractal && $(MAKE))
	echo ''lib : ok''