CFLAGS += -Wall -Wextra -Werror -std=c89 -pedantic -Wno-unused-parameter -g -I./src/include
CFLAGS += -DDEBUG
#CFLAGS += -DFPS_ECO
#CFLAGS += -DDEBUG_BOX
#CFLAGS += -O2


UNIX_CC = gcc
UNIX_SDL2_CFLAGS := $(shell sdl2-config --cflags)
UNIX_SDL2_LDFLAGS := $(shell sdl2-config --libs) -lSDL2_image -lm


all: unix

unix: CC = $(UNIX_CC)
unix: CFLAGS += $(UNIX_SDL2_CFLAGS)
unix: LDFLAGS += $(UNIX_SDL2_LDFLAGS)
unix: game


game: example/game.o $(addprefix src/, Base.o CList.o Entity.o QTree.o Window.o)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

example/game.o: example/game.c $(addprefix src/include/, engine.h Base.h CList.h Entity.h QTree.h Window.h)
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

%/Base.o: %/Base.c $(addprefix %/include/, Base.h CList.h Entity.h QTree.h Window.h)
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

%/CList.o: %/CList.c $(addprefix %/include/, Base.h CList.h Entity.h layer.h)
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

%/Entity.o: %/Entity.c $(addprefix %/include/, Base.h CList.h Entity.h layer.h QTree.h Window.h)
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

%/QTree.o: %/QTree.c $(addprefix %/include/, Base.h CList.h Entity.h QTree.h Window.h)
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

%/Window.o: %/Window.c $(addprefix %/include/, Base.h Entity.h Window.h)
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

clean:
	rm -rf src/*.o example/*.o game 
