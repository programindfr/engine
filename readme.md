@mainpage readme.md

# engine

Welcome to the engine's documentation.

## Build

The engine can compile/cross-compile to Linux/Unix and Windows.

### Linux/Unix

In order to build the engine for Linux/Unix you need `make`, `gcc`, `libsdl2-dev` and `libsdl2-image-dev`. You can install them on Debian-based systems with `apt install make gcc libsdl2-dev libsdl2-image-dev`.

```bash
# To build for Linux/Unix
make unix
```

### Windows

In order to build the engine for Windows you need `make`, `x86_64-w64-mingw32-gcc` and some SDL files. You can install them on Debian-based systems with `apt install make gcc-mingw-w64-x86-64` and download `SDL2-devel-2.x.x-mingw.tar.gz` at <https://github.com/libsdl-org/SDL/releases>. If the archive is not extracted in your home directory, the `W64_SDL2_PATH` variable in the Makefile must be changed according to where the archive is extracted. To compile you have to edit the file `SDL2-devel-2.x.x-mingw/SDL2-2.x.x/x86_64-w64-mingw32/bin/sdl2-config`.

```md
# Line 49, change this
echo -I${prefix}/include/SDL2  -Dmain=SDL_main

# To this
echo -I${prefix}/include  -Dmain=SDL_main
```

Now you can compile.

```bash
# To build for Windows
make w64
```

### Documentation

If you want to build the documentation you will need `doxygen` (install it with `apt install doxygen`).

```bash
# To build the documentation
make doc
```

That's it !

## Supported platforms

The project is developed in ANSI C on a Debian system whithout any specific headers, so it should be compilling and running on all Linux/Unix systems. Support for Windows is tested thought Wine.
