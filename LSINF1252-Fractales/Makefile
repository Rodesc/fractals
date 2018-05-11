all: main.o additionals.o main lib
	@(echo ''Compilation all ... '')
	@(echo ''OK'')

main: main.o additionals.o lib 
	@(echo ''Compilation main ...'')
	@(gcc -g -Wall -o main main.o additionals.o libfractal/fractal.o libfractal/tools.o -lSDL -lpthread)
	@(echo ''OK'')

main.o: main.c 
	@(echo ''Compilation main.o ...'')
	gcc -Wall -g -c main.c -lpthread
	@(echo ''OK'')

additionals.o: additionals.c main.c
	@(echo ''Compilation additionals.o ...'')
	gcc -Wall -g -c additionals.c main.c -lpthread -lSDL
	@(echo ''OK'')

clean:
	@rm -rf *.o libfractal/*.o libfractal/*.a tests/*.o *.bmp main tests/test1
	@echo "Cleaned"
lib:
	@(echo ''Compilation lib ...'')
	@(cd libfractal && make)
	@(echo ''OK'')
tests:
	@echo "Testing"
	@(cd tests && make)