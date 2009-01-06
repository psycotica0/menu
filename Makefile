menu: menu.o
	gcc -o menu menu.o

menu.o: menu.c
	gcc -c -Wall -ansi -pedantic -o menu.o menu.c

.PHONY: clean test run

clean:
	rm -f menu
	rm -f menu.o

test: run clean

run: menu
	ls | ./menu | xargs ls -l
