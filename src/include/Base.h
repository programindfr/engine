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
	@file Base.h
*/

#ifndef __BASE_H__
#define __BASE_H__

#include <SDL2/SDL.h>

#define LOG_ERROR(RETNO, NAME) if (RETNO) SDL_LogError(SDL_LOG_CATEGORY_ERROR, "l:%d no:%d %s", __LINE__, RETNO, NAME)

/**
	@enum retno
	@brief Class ctor/dtor return state
*/
typedef enum retno {
	SUCCESS = 0,
	FAILURE
} retno_t;

/**
	@enum type
	@brief Class type enum
*/
typedef enum type {
	CLIST	 = 0x1,
	QTREE	 = 0x2,
	WINDOW	= 0x4,
	ENTITY	= 0x8
} type_t;

typedef union clist_u CList_t;
typedef union entity_u Entity_t;
typedef union qtree_u QTree_t;
typedef union window_u Window_t;

#define BASE_CLASS \
type_t type;

void *new(type_t type, ...);
void delete(void *ptr);

#endif/*__BASE_H__*/
