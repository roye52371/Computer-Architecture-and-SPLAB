all:task1c
task1c: task1c.o
	gcc -m32 -g -Wall -o task1c task1c.o
task1c.o: task1c.c
	gcc -g -Wall -m32  -c -o task1c.o task1c.c
.PHONY: clean
clean: 
	rm -f *.o task1c
