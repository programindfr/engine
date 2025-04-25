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
	@file CList.h
*/

#ifndef __CLIST_H__
#define __CLIST_H__

#include <Base.h>
#include <layer.h>

#define CList() new(CLIST)

typedef struct clist_block {
	void *content;
	struct clist_block *next;
} clist_block_t;

#define CLIST_CLASS \
clist_block_t *head;\
\
void *(*pop)(CList_t *self);\
void (*empty)(CList_t *self);\
void (*push)(CList_t *self, void *content);\
void (*remove)(CList_t *self, void *content);\
void *(*iter)(CList_t *self, clist_block_t **block);\
void (*entityUpdateAndDraw)(CList_t *self, layer_t updateLayer, layer_t drawLayer);

typedef struct clist_s {
	BASE_CLASS
	CLIST_CLASS
} clist_t;

union clist_u {
	type_t type;
	clist_t clist;
};

retno_t clist_t__ctor(CList_t *self);
retno_t clist_t__dtor(CList_t *self);

#endif/*__CLIST_H__*/
