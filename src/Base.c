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
	@file Base.c
*/

#include <Base.h>
#include <CList.h>
#include <Entity.h>
#include <QTree.h>
#include <SDL2/SDL.h>
#include <stdarg.h>
#include <stdlib.h>
#include <Window.h>

void *new(type_t type, ...)
{
	void	   *self = NULL;
	va_list	arguments;
	retno_t	retno;

	switch (type)
	{
	case CLIST:
		self = calloc(1, sizeof(CList_t));
		break;

	case QTREE:
		self = calloc(1, sizeof(QTree_t));
		break;

	case WINDOW:
		self = calloc(1, sizeof(Window_t));
		break;

	case ENTITY:
		self = calloc(1, sizeof(Entity_t));
		break;

	default:
		return self;
	}
	
	va_start(arguments, type);
	
	if (type & CLIST)
	{
		retno = clist_t__ctor((CList_t *) self);
		LOG_ERROR(retno, "clist_t__ctor");
	}
	
	if (type & QTREE)
	{
		((QTree_t *) self)->qtree.rect = va_arg(arguments, SDL_FRect);
		retno = qtree_t__ctor((QTree_t *) self);
		LOG_ERROR(retno, "qtree_t__ctor");
	}
	
	if (type & WINDOW)
	{
		retno = window_t__ctor((Window_t *) self);
		LOG_ERROR(retno, "window_t__ctor");
	}
	
	if (type & ENTITY)
	{
		((Entity_t *) self)->entity.window = va_arg(arguments, Window_t *);
		((Entity_t *) self)->entity.position.x = va_arg(arguments, double);
		((Entity_t *) self)->entity.position.y = va_arg(arguments, double);
		((Entity_t *) self)->entity.position.layer = va_arg(arguments, layer_t);
		((Entity_t *) self)->entity.health.rect = va_arg(arguments, SDL_FRect);
		((Entity_t *) self)->entity.graphics.path = va_arg(arguments, char *);
		retno = entity_t__ctor((Entity_t *) self);
		LOG_ERROR(retno, "entity_t__ctor");
	}
	
	va_end(arguments);
	*(type_t *) self = type;
	
	return self;
}

void delete(void *self)
{
	if (*(type_t *) self & CLIST)
		clist_t__dtor((CList_t *) self);
	
	if (*(type_t *) self & QTREE)
		qtree_t__dtor((QTree_t *) self);
	
	if (*(type_t *) self & WINDOW)
		window_t__dtor((Window_t *) self);
	
	if (*(type_t *) self & ENTITY)
		entity_t__dtor((Entity_t *) self);
	
	free(self);
}
