main: main.o input.o handling.o
	gcc -o main main.o input.o handling.o
main.o: main.c input.h handling.h structs.h
	gcc -c main.c
handling.o: handling.h handling.c structs.h
	gcc -c handling.c
input.o: input.h input.c structs.h
	gcc -c input.c
clean:
	rm -rf *.o 
