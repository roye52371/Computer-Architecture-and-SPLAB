all:task4
task4: LineParser.o task4.o
	gcc -m32 -g -Wall -o task4 LineParser.o task4.o
LineParser.o: LineParser.c
	gcc -g -Wall -m32  -c -o LineParser.o LineParser.c
task4.o: task4.c
	gcc -g -Wall -m32  -c -o task4.o task4.c
.PHONY: clean
clean: 
	rm -f *.o task4
