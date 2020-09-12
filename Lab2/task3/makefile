all:task3
task3: menu_map.o
	gcc -m32 -g -Wall -o task3 menu_map.o
menu_map.o: menu_map.c
	gcc -g -Wall -m32  -c -o menu_map.o menu_map.c
.PHONY: clean
clean: 
	rm -f *.o task3
