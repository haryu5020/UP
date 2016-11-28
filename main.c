#include <ncurses.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#define KEY_ESC 27
#define KEY_COLON 58
#define KEY_INSERT 105
#define KEY_ENT 10
#define KEY_W 119
#define KEY_Q 113
#define TRUE 1
#define FALSE 0
#define FILE_MODE 0644

int cursorX = 0;
int cursorY = 1;


void initializeCurses();
void setPrompt(char *prompt, WINDOW* cmd);
void moveCursor(int ch, int dy, int dx);
int getLastChar(WINDOW *wnd, int mx);

int main(int argc, char *argv[])
{
	int fd;		//file descriptor
	int ch;		//keyboard input char
	int ch2;
	int doExit = FALSE;
	int mode = 0;
	WINDOW *editWindow;
	WINDOW *cmdWindow;
	int load[2];

	int i = 0;
	int j = 0;
	int count;
	int my;
	int mx;
	int *buf;
	
	/* If file is not exist, make a file */
	if ( fd = open(argv[1], O_RDWR | O_APPEND) < 0 ){
		if( fd = creat(argv[1], FILE_MODE) < 0 ){
			printf("Create Error");
		}
	}

	/* WINDOW START */
	initializeCurses();

	/* Divide Screen, Edit / Command */
	editWindow = derwin(stdscr, LINES - 1, COLS, 0, 0);
	cmdWindow = derwin(stdscr, 1, COLS, LINES - 1, 0);
	refresh();

	/* Permit Special Character */
	keypad(editWindow, TRUE);
	keypad(cmdWindow, TRUE);

	/* File Open and Load */
	fd = open(argv[1], O_RDWR | O_APPEND);
	while( read(fd, load, 1) > 0 )
		waddch(editWindow, load[0]); 
	close(fd);
	wmove(editWindow, 0, 0);
	
	/* Select Mode */
	while(doExit != TRUE)
	{
		ch = getch();	
		if( mode == 0 ) 	//Command mode
		{
			switch(ch)
			{
			case KEY_COLON :
				mode = 1;
				setPrompt(":", cmdWindow);
				break;
			case KEY_INSERT :
				mode = 2;
				setPrompt(" -- INSERT MODE -- ", cmdWindow);
				break;
			default :
				moveCursor(ch, cursorY, cursorX);
				break;
			}
			refresh();
		}	
		else if( mode == 1 )	// Colon Mode
		{
			switch(ch)
			{
			case KEY_ESC :
				mode = 0;
				setPrompt(" ",cmdWindow);
				break;
			case KEY_W:
				ch2 = KEY_W;
				setPrompt(":w", cmdWindow);
				break;
			case KEY_Q:
				ch2 = KEY_Q;
				setPrompt(":q", cmdWindow);
				break;	
			case KEY_ENT:
				if(ch2 == KEY_W)
				{
					//저장하는 함수
				}
				else if(ch2 == KEY_Q)
				{		
					doExit = TRUE;	//종료 함수
				}
				break;
			default :
				break;
			}
			refresh();
					
		}
		else if( mode == 2 )	// Insert Mode
		{
			switch(ch)
			{
			case KEY_ESC :
				mode = 0;
				setPrompt(" ",cmdWindow);
				break;
			case KEY_ENT :
				getyx(editWindow, cursorY, cursorX);
				getmaxyx(editWindow, my, mx);
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
				break;
			case KEY_UP :
			case KEY_DOWN :
			case KEY_LEFT :
			case KEY_RIGHT :
				moveCursor(ch, cursorY, cursorX);
				break;
			case KEY_BACKSPACE :
				getyx(editWindow, cursorY, cursorX);
				wmove(editWindow, cursorY, --cursorX);
				if( cursorX < 0 ) {
					--cursorY;
					while (cursorX != '\n'){
						cursorX++;
					}
					wmove(editWindow, cursorY, cursorX);
					wdelch(editWindow);
				}
				else
					wdelch(editWindow);
				break;
			default :
				printw("%c",ch);
				cursorX += 1;
				break;
			}	
			refresh();
		}
	}

	endwin();
}	
void initializeCurses()
{
	initscr();
	raw();
	keypad(stdscr, TRUE);
	noecho();
}

void moveCursor(int ch, int dy, int dx)
{
	switch(ch)
	{
	case KEY_UP :
		move(dy - 1, dx);
		cursorY = dy - 1;
		cursorX = dx;
		break;
	case KEY_DOWN :
		move(dy + 1, dx);
		cursorY = dy + 1;
		cursorX = dx;
		if(cursorY == (LINES - 1))
		{
			move(--cursorY, 0);
		} 
		break;
	case KEY_LEFT :
		move(dy, dx - 1);
		cursorY = dy;
		cursorX = dx - 1;
		break;
	case KEY_RIGHT :
		move(dy, dx + 1);
		cursorY = dy;
		cursorX = dx + 1;
		break;
	default :
		break;
	}
}

void setPrompt(char* prompt, WINDOW* cmd)
{   
	int y,x;
	getyx(cmd, y, x); 
	wmove(cmd, 0, 0);
	wclrtoeol(cmd);
	mvwprintw(cmd, 0, 0, prompt);
	wmove(cmd, y, x);
	wrefresh(cmd);
}// bottom line write Command Line

int getLastChar(WINDOW *wnd, int mx)
{
	int y, x;
	getyx(wnd, y, x);
	x = mx - 1;
	move(y, x);
	while( isspace(winch(wnd)) && x > 0 )
		move(y, 0);
	move(y, 0);
	return x;
}

int getLastLine(WINDOW *wnd, int my, int mx) {
	int i, j;
	move(my - 1, mx - 1);
	for (i = my - 1; i >= 0; i--) {
		for (j = mx - 1; j >= 0; j--) {
			if (isspace(winch(wnd))||winch(wnd)=='~')
				move(i, j);
			else
				return i;
		}
	}
	return 0;
}

void saveFile(WINDOW *wnd, char *argv[])
{
	int lChar;
	int lLine;
	int fd;
	int my;
	int mx;
	int y = 0;
	int x = 0;
	char ch[2];

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
