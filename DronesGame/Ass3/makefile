Sall: ass3

ass3: ass3.o drone.o printer.o scheduler.o target.o
	gcc -m32 -g -Wall -o ass3 ass3.o drone.o printer.o scheduler.o target.o

drone.o: drone.s
	nasm -f elf drone.s -o drone.o

printer.o: printer.s
	nasm -f elf printer.s -o printer.o

scheduler.o: scheduler.s
	nasm -f elf scheduler.s -o scheduler.o

target.o: target.s
	nasm -f elf target.s -o target.o


ass3.o: ass3.s
	nasm -f elf ass3.s -o ass3.o
	
.PHONY: clean
clean: 
	rm -f *.o ass3
