.PHONY: clean all final

battleship: main.o computer.o actions.o
	gcc -o battleship main.o computer.o actions.o
actions.o: source/actions.c source/header.h
	gcc -Wall -Wno-char-subscripts -c source/actions.c
computer.o: source/computer.c source/header.h
	gcc -Wall -Wno-char-subscripts -c source/computer.c
main.o: source/main.c source/header.h
	gcc -Wall -Wno-format -c source/main.c

clean:
	rm -f main.o computer.o actions.o battleship

all: clean battleship

final: all
	rm -f main.o computer.o actions.o
