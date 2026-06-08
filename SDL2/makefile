ifeq ($(OS),Windows_NT)
    CC = gcc
    RM = del /Q
    EXE = .exe
    INCLUDE = -I./include
    LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net -lSDL2_ttf -lSDL2_mixer
    LIBS =
else
    CC = gcc
    RM = rm -f
    EXE =
    INCLUDE = -I/opt/homebrew/include -I./include
    LIBS = -L/opt/homebrew/lib
    LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_net -lSDL2_ttf -lSDL2_mixer -lm
endif

SRCDIR = ./src
CFLAGS = -g $(INCLUDE)

.PHONY: all game server clean

all: game server

game:
	$(CC) $(CFLAGS) $(SRCDIR)/main.c $(SRCDIR)/player.c -o game$(EXE) $(LIBS) $(LDFLAGS)

server:
	$(CC) $(CFLAGS) $(SRCDIR)/server.c -o server$(EXE) $(LIBS) $(LDFLAGS)

clean:
ifeq ($(OS),Windows_NT)
	-$(RM) game$(EXE) 2>nul
	-$(RM) server$(EXE) 2>nul
else
	$(RM) game$(EXE) server$(EXE)
endif