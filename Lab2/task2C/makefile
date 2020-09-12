all:task2C
task2C: menu_map.o
	gcc -m32 -g -Wall -o task2C menu_map.o
menu_map.o: menu_map.c
	gcc -g -Wall -m32  -c -o menu_map.o menu_map.c
.PHONY: clean
clean: 
	rm -f *.o task2C
