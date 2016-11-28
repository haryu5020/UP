void setPrompt(char *prompt, WINDOW* wnd);
void insertModeEnter(WINDOW* wnd);
void moveCursor(int ch, int dy, int dx);
void insertModeBackspace(WINDOW* wnd, int dy, int dx);
void insertModePrintChar(WINDOW* wnd, int ch);
