CC = gcc
CFLAGS = -W -Wall

TARGET = vi_editor
OBJECTS = init.o colon.o edit.o main.o

LIBS = -lncurses

all : $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean :
	rm -f *.o vi_editor
