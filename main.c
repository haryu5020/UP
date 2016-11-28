#include <ncurses.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#include "init.h"
#include "edit.h"
#include "colon.h"

int cursorX = 0;
int cursorY = 1;

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
	wrefresh(editWindow);
	wrefresh(cmdWindow);

	setEditWindow(editWindow);
	wrefresh(editWindow);

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
				insertModeEnter(editWindow);
			case KEY_UP :
			case KEY_DOWN :
			case KEY_LEFT :
			case KEY_RIGHT :
				moveCursor(ch, cursorY, cursorX);
				break;
			case KEY_BACKSPACE :
				insertModeBackspace(editWindow, cursorY, cursorX);
				break;
			default :
				insertModePrintChar(editWindow,ch);
				break;
			}	
		}
	}
	endwin();
	return 0;
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
