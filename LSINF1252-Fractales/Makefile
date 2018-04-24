
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
all: main.o fractal_test1 lib
	@(echo ''Compilation all ... '')
	@(echo ''OK'')

fractal_test1: main.o lib 
	@(echo ''Compilation fractal_test1 ...'')
	@(gcc -g -Wall -o fractal_test1 main.o libfractal/fractal.o libfractal/tools.o -lSDL -lpthread)
	@(echo ''OK'')

main.o: main.c lib
	@(echo ''Compilation main.o ...'')
	gcc -Wall -g -c main.c -lpthread
	@(echo ''OK'')

lib:
	@(echo ''Compilation lib ...'')
	@(cd libfractal && $(MAKE))
	@(echo ''OK'')