all:task2b
task2b: task2b.o
	gcc -m32 -g -Wall -o task2b task2b.o
task2b.o: task2b.c
	gcc -g -Wall -m32  -c -o task2b.o task2b.c
.PHONY: clean
clean: 
	rm -f *.o task2b
