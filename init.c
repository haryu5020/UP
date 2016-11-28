#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "init.h"
#include "edit.h"

void initializeCurses()
{
	initscr();
        raw();
        keypad(stdscr, TRUE);
        noecho();
}

void setEditWindow(WINDOW *wnd)
{
	int my, mx;
	int y;
	
	getmaxyx(wnd, my, mx);
	y = getLastLine(wnd, my, mx);
	for(y; y <= my;)
	{
		mvwprintw(wnd, ++y, 0, "%s","~");
	}
	wmove(wnd, 0, 0);
	wrefresh(wnd);
}
