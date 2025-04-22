AC = nasm
CC = gcc
all: corox86_64.o
	$(CC) -no-pie -o out  example.c corox86_64.o 
	rm -f *.o

corox86_64.o:
	$(AC) -f elf64 corox86_64.asm -o corox86_64.o
		

clean:
	rm *.o

