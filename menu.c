#include <ncurses.h>

int main () {
	initscr();
	char inp[4];

	getnstr(&inp,3);

	endwin();
	printf("You typed: %s\n",inp);
	return 0;
}
