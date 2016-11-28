#include <ncurses.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "init.h"
#include "colon.h"
#include "edit.h"

void setPrompt(char* prompt, WINDOW* wnd)
{
	int y, x;
	getyx(wnd, y, x);
	wmove(wnd, 0, 0);
	wclrtoeol(wnd);
	mvwprintw(wnd, 0, 0, prompt);
	wmove(wnd, y, x);
	wrefresh(wnd);
}

void insertModeEnter(WINDOW *wnd)
{
	int i, j = 0;
	int my, mx;
	int* buf;	
	int count;
	int y, x;

	getyx(wnd, y, x);

	getmaxyx(wnd, my, mx);
	buf = (int *)malloc(sizeof(int*)*mx);

        count = mx - x;

        while( count > 0 )
        {
        	buf[i++] = winch(wnd);
                wdelch(wnd);
                count--;
        }
        buf[i] = '\0';
        wmove(wnd, ++y, 0);
        winsertln(wnd);
        while( buf[j] != '\0' )
        {
        	waddch(wnd, buf[j]);
                j++;
        }
        wmove(wnd, y, 0);
	if( winch(wnd) == '~'){
		wdelch(wnd);
	}
}

void insertModeBackspace(WINDOW *wnd, int dy, int dx)
{
	getyx(wnd, dy, dx);
	wmove(wnd, dy, --dx);
	if( dx < 0 ) {
		--dy;
		while( dx != '\n' ){
			dx++; 
		}
		wmove(wnd, dy, dx);
		wdelch(wnd);
	}
	else
		wdelch(wnd);
}

void insertModePrintChar(WINDOW* wnd,int ch)
{
	int x,y;
	getyx(wnd, y, x);
	wprintw(wnd, ch);
	x++;
	wmove(wnd, y, x);
}
