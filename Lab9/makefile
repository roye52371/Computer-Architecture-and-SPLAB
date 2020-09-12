all: task3
task3: task3.o
	ld -m elf_i386 task3.o -o task3
task3.o: task3.s
	nasm -f elf32 task3.s -o task3.o	

.PHONY: clean
clean: 
	rm -f *.o task3
