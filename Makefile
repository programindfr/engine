CC = gcc
SDL2_CFLAGS := $(shell sdl2-config --cflags)
SDL2_LDFLAGS := $(shell sdl2-config --libs) -lSDL2_image
COLOR_GREEN := "\033[0;32m"
COLOR_YELLOW := "\033[0;33m"
COLOR_END := "\033[0m"

CFLAGS += -Wall -Wextra -std=c89 -pedantic -Wno-unused-parameter
CFLAGS += -I.
CFLAGS += -g
CFLAGS += $(SDL2_CFLAGS)
LDFLAGS += $(SDL2_LDFLAGS)


all: game

game: game.o
	@$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "LD $@ "$(COLOR_GREEN)"SUCCESS"$(COLOR_END)

game.o: game.c engine.h
	@$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)
	@echo "CC $@ "$(COLOR_GREEN)"OK"$(COLOR_END)

clean:
	@echo $(COLOR_YELLOW)"CLEANING"$(COLOR_END)
	rm -f *.o game
