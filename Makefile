menu: menu.o
	gcc -lncurses -o menu menu.o

menu.o: menu.c
	gcc -c -o menu.o menu.c

.PHONY: clean test run

clean:
	rm -f menu
	rm -f menu.o

test: run clean

run: menu
	./menu
