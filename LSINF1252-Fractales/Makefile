all: main.o additionals.o fractal_test1 lib
	@(echo ''Compilation all ... '')
	@(echo ''OK'')

fractal_test1: main.o additionals.o lib 
	@(echo ''Compilation fractal_test1 ...'')
	@(gcc -g -Wall -o fractal_test1 main.o additionals.o libfractal/fractal.o libfractal/tools.o -lSDL -lpthread)
	@(echo ''OK'')

main.o: main.c
	@(echo ''Compilation main.o ...'')
	gcc -Wall -g -c main.c -lpthread
	@(echo ''OK'')

additionals.o: additionals.c 
	@(echo ''Compilation additionals.o ...'')
	gcc -Wall -g -c additionals.c -lpthread -lSDL
	@(echo ''OK'')

lib:
	@(echo ''Compilation lib ...'')
	@(cd libfractal && make)
	@(echo ''OK'')