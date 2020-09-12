all:task4
task4: task4.o
	gcc -m32 -g -Wall -fno-pie -o task4 task4.o

task4.o: task4.c
	gcc -g -Wall -m32  -c -fno-pie -o task4.o task4.c
.PHONY: clean
clean: 
	rm -f *.o task4
