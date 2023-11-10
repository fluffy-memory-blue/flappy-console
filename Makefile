CC = gcc
CFLAGS = -g

OBJ = \
	main.o \
	game.o \
	input_ncurses.o \
	renderer_ncurses.o

HEADERS = \
	game.h \
	input_ncurses.h \
	renderer_ncurses.h

LIBS = -lm -lncurses

APP = flappy-console

%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJ)
	$(CC) -o $(APP) $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o $(APP)