/*
c89 2d game engine build with sdl2.
Copyright (C) 2025 Programind

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/**
	@file Window.h
*/

#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <Base.h>
#include <SDL2/SDL.h>
#include <stdint.h>

#define Window() new(WINDOW)

typedef struct window_camera {
	SDL_FRect rect;
	SDL_Texture *texture;
	SDL_Texture *shadow;
	SDL_Color lighting;
} window_camera_t;

#define WINDOW_CLASS \
uint64_t		   time;\
uint64_t		   deltatime;\
SDL_Event		  event;\
SDL_Window		 *window;\
SDL_Renderer	   *renderer;\
window_camera_t	camera;\
\
void		 (*setLighting)(Window_t *self, SDL_Color color);\
void		 (*putOnCamera)(Window_t *self, Entity_t *content);\
uint8_t	  (*update)(Window_t *self);\
uint64_t	 (*getDeltatime)(Window_t *self);\
SDL_Event	(*getEvent)(Window_t *self);

typedef struct window_s {
	BASE_CLASS
	WINDOW_CLASS
} window_t;

union window_u {
	type_t type;
	window_t window;
};

retno_t window_t__ctor(Window_t *self);
retno_t window_t__dtor(Window_t *self);

#endif/*__WINDOW_H__*/
