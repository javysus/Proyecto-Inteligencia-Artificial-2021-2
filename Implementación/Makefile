main: main.o alsp.o 
	gcc main.o alsp.o -o main
main.o: main.c
	gcc -c main.c -Wall
alsp.o: alsp.c alsp.h
	gcc -c alsp.c -Wall
clean:
	rm -f *.o
