COLOR_GREEN := "\033[0;32m"
COLOR_YELLOW := "\033[0;33m"
COLOR_END := "\033[0m"

CFLAGS += -Wall -Wextra -std=c89 -pedantic -Wno-unused-parameter -g -I.


UNIX_CC = gcc
UNIX_SDL2_CFLAGS := $(shell sdl2-config --cflags)
UNIX_SDL2_LDFLAGS := $(shell sdl2-config --libs) -lSDL2_image

W64_CC = x86_64-w64-mingw32-gcc
W64_SDL2_PATH = ~/SDL2-devel-2.30.7-mingw/SDL2-2.30.7/x86_64-w64-mingw32
W64_SDL2_IMAGE_PATH = ~/SDL2-image-devel-2.8.5-mingw/SDL2_image-2.8.5/x86_64-w64-mingw32
W64_SDL2_CFLAGS := $(shell $(W64_SDL2_PATH)/bin/sdl2-config --cflags) -I$(W64_SDL2_IMAGE_PATH)/include
W64_SDL2_LDFLAGS := $(shell $(W64_SDL2_PATH)/bin/sdl2-config --libs) -L$(W64_SDL2_IMAGE_PATH)/lib -lSDL2_image


all:

unix: CC = $(UNIX_CC)
unix: CFLAGS += $(UNIX_SDL2_CFLAGS)
unix: LDFLAGS += $(UNIX_SDL2_LDFLAGS)
unix: game

w64: CC = $(W64_CC)
w64: CFLAGS += $(W64_SDL2_CFLAGS)
w64: LDFLAGS += $(W64_SDL2_LDFLAGS)
w64: game
	ln -sf $(W64_SDL2_PATH)/bin/SDL2.dll SDL2.dll
	ln -sf $(W64_SDL2_IMAGE_PATH)/bin/SDL2_image.dll SDL2_image.dll

game: example/game.o
	@$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "LD $@ "$(COLOR_GREEN)"SUCCESS"$(COLOR_END)

example/game.o: example/game.c engine.h
	@$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)
	@echo "CC $@ "$(COLOR_GREEN)"OK"$(COLOR_END)

doc:
	ln -sf html/index.html index.html
	doxygen -x
	doxygen

clean:
	@echo $(COLOR_YELLOW)"CLEANING"$(COLOR_END)
	rm -rf *.o game game.exe SDL2.dll SDL2_image.dll html index.html

archive: clean
	git archive -o ar/engine$(shell date "+%Y%m%d").tar.gz main
