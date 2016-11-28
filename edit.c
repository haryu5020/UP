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
	wrefrech(wnd);
}

void insertModeEnter(WINDOW *wnd, int dy, int dx)
{
	int my, mx;
	int* buf;	
	int count;

	getyx(wnd, dy, dx);
	getmaxyx(wnd, my, mx);
	buf = (int *)malloc(sizeof(int*)*mx);

        count = mx - cursorX;

        while( count > 0 )
        {
        	buf[i++] = winch(editWindow);
                wdelch(editWindow);
                count--;
        }
        buf[i] = '\0';
        wmove(editWindow,++cursorY, 0);
        winsertln(editWindow);
        while( buf[j] != '\0' )
        {
        	waddch(editWindow, buf[j]);
                j++;
        }
        wmove(editWindow, cursorY, 0);
}

void insertModeBackspace(WINDOW *wnd, int dy, int dx)
{
	getyx(wnd, dy, dx);
	wmove(wnd, dy, --dx);
	if( dx < 0 ) {
		--dy;
		while( cursorX != '\n' ){
			dx++ 
		}
		wmove(wnd, dy, dx);
		wdelch(wnd);
	}
	else
		wdelch(wnd);
}

void insertModePrintChar()
