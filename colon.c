#include <ncurses.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>

#include "colon.h"
#include "init.h"
#include "edit.h"

/* Get Last Line Coordinate */
int getLastLine(WINDOW *wnd, int my, int mx) {
	int i, j;
	wmove(wnd, my - 1, mx - 1);
	for (i = my - 1; i >= 0; i--) {
		for (j = mx - 1; j >= 0; j--) {
			if (isspace(winch(wnd))||winch(wnd)=='~')
				wmove(wnd, i, j);
			else
				return i;
		}
	}
	return 0;
}

/* Get Last Char Coordinate In One Line*/
int getLastChar(WINDOW *wnd, int mx) {
	int y, x;
	getyx(wnd, y, x);
	x = mx - 1;
	wmove(wnd, y, x);
	while (isspace(winch(wnd)) && x > 0)
		wmove(wnd, y, --x);
	wmove(wnd, y, 0);
	return x;
}

void saveFile(WINDOW *wnd, char *argv[]) {
	int lChar;
	int lLine;
	int fd;
	int my;
	int mx;
	int y = 0;
	int x = 0;
	char ch[2];
	
	/* Overwrite */
	fd = open(argv[0], O_RDWR | O_APPEND | O_TRUNC);

	getmaxyx(wnd, my, mx);
	lLine = getLastLine(wnd, my, mx);
	wmove(wnd, 0, 0);
	while (y <= lLine) {
		x = 0;
		lChar = getLastChar(wnd, mx);
		while (x <= lChar) {
			ch[0] = winch(wnd);
			write(fd, ch, 1);
			wmove(wnd, y, ++x);
		}
		ch[0] = '\n';
		write(fd, ch, 1);
		wmove(wnd, ++y, 0);
	}
}
