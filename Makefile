COLOR_GREEN := "\033[0;32m"
COLOR_YELLOW := "\033[0;33m"
COLOR_END := "\033[0m"

CFLAGS += -Wall -Wextra -std=c89 -pedantic -Wno-unused-parameter -g -I. -DFPS_ECO


UNIX_CC = gcc
UNIX_SDL2_CFLAGS := $(shell sdl2-config --cflags)
UNIX_SDL2_LDFLAGS := $(shell sdl2-config --libs) -lSDL2_image

ifeq ($(shell find | grep ./lib), ./lib)
	W64_CC = x86_64-w64-mingw32-gcc
	W64_SDL2_CFLAGS := $(shell ./lib/bin/sdl2-config --cflags)
	W64_SDL2_LDFLAGS := $(shell ./lib/bin/sdl2-config --libs) -lSDL2_image
endif


all: unix

unix: CC = $(UNIX_CC)
unix: CFLAGS += $(UNIX_SDL2_CFLAGS)
unix: LDFLAGS += $(UNIX_SDL2_LDFLAGS)
unix: game multi

w64: CC = $(W64_CC)
w64: CFLAGS += $(W64_SDL2_CFLAGS)
w64: LDFLAGS += $(W64_SDL2_LDFLAGS)
w64: game multi
	ln -sf ./lib/bin/SDL2.dll SDL2.dll
	ln -sf ./lib/bin/SDL2_image.dll SDL2_image.dll

dl_w64_lib:
	./dl_w64_lib.py

game: example/game.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "LD $@ "$(COLOR_GREEN)"SUCCESS"$(COLOR_END)

example/game.o: example/game.c engine.h
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)
	@echo "CC $@ "$(COLOR_GREEN)"OK"$(COLOR_END)

multi: example/a.o example/b.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "LD $@ "$(COLOR_GREEN)"SUCCESS"$(COLOR_END)

example/a.o: example/a.c engine.h example/b.h
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)
	@echo "CC $@ "$(COLOR_GREEN)"OK"$(COLOR_END)

example/b.o: example/b.c engine.h example/b.h
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)
	@echo "CC $@ "$(COLOR_GREEN)"OK"$(COLOR_END)

doc:
	ln -sf html/index.html index.html
	doxygen -x
	doxygen

clean:
	@echo $(COLOR_YELLOW)"CLEANING"$(COLOR_END)
	rm -rf example/*.o game game.exe multi multi.exe SDL2.dll SDL2_image.dll html index.html

archive: clean
	git archive -o ar/engine$(shell date "+%Y%m%d").tar.gz main
