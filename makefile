myset: set.o myset.o
	gcc -g -ansi -pedantic -Wall set.o myset.o -o myset
set.o: set.c set.h
	gcc -c -ansi -pedantic -Wall set.c -o set.o
myset.o: myset.c set.h
	gcc -c -pedantic -ansi -Wall myset.c -o myset.o	
