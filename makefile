.PHONY: clean all final

battleship: main.o computer.o actions.o
	gcc -o battleship main.o computer.o actions.o
actions.o: source/actions.c source/header.h
	gcc -c source/actions.c -Wall -Wno-char-subscripts
computer.o: source/computer.c source/header.h
	gcc -c source/computer.c -Wall -Wno-char-subscripts
main.o: source/main.c source/header.h
	gcc -c source/main.c -Wall -Wno-format

clean:
	rm -f main.o computer.o actions.o battleship

all: clean battleship

final: all
	rm -f main.o computer.o actions.o