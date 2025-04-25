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
	@file QTree.h
*/

#ifndef __QTREE_H__
#define __QTREE_H__

#include <Base.h>
#include <SDL2/SDL.h>
#include <stdint.h>

#define QTree(RECT) new(QTREE, RECT)

#define QTREE_CLASS \
QTree_t	  *tree[4];\
Entity_t	 *content[4];\
SDL_FRect	rect;\
\
void	   (*update)(QTree_t *self);\
void	   (*draw)(QTree_t *self, Window_t *window);\
void	   (*insert)(QTree_t *self, Entity_t *content);\
CList_t	*(*fetch)(QTree_t *self, SDL_FRect rect);\
uint8_t	(*remove)(QTree_t *self, Entity_t *content);

typedef struct qtree_s {
	BASE_CLASS
	QTREE_CLASS
} qtree_t;

union qtree_u {
	type_t type;
	qtree_t qtree;
};

retno_t qtree_t__ctor(QTree_t *self);
retno_t qtree_t__dtor(QTree_t *self);

#endif/*__QTREE_H__*/
