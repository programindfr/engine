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
	@file CList.c
*/

#include <Base.h>
#include <CList.h>
#include <Entity.h>
#include <layer.h>
#include <stdlib.h>

/**
	@relates clist_s
	@fn void clist_t__push(CList_t *self, void *content)
	@brief Head push
	@param self Object pointer
	@param content Element pointer
	@return void
*/
void clist_t__push(CList_t *self, void *content)
{
	clist_block_t *block = NULL;
	
	block = calloc(1, sizeof(clist_block_t));
	
	block->content = content;
	block->next = self->clist.head;
	
	self->clist.head = block;
}

/**
	@relates clist_s
	@fn void *clist_t__pop(CList_t *self)
	@brief Head pop
	@param self Object pointer
	@return Head element pointer
*/
void *clist_t__pop(CList_t *self)
{
	void			 *content = NULL;
	clist_block_t	*block = NULL;
	
	if (!self->clist.head)
		return content;
	
	block = self->clist.head;
	content = block->content;
	self->clist.head = block->next;
	
	free(block);
	
	return content;
}

/**
	@relates clist_s
	@fn void *clist_t__iter(CList_t *self, clist_block_t **block)
	@brief Iterate chained list elements
	@param self Object pointer
	@param block Link pointer
	@return Element pointer
	
	@todo explain/example
*/
void *clist_t__iter(CList_t *self, clist_block_t **block)
{
	if (!*block)
		*block = self->clist.head;
	else
		*block = (*block)->next;
	
	if (*block)
		return (*block)->content;
	
	return NULL;
}

/**
	@relates clist_s
	@fn void clist_t__remove(CList_t *self, void *content)
	@brief Remove element
	@param self Object pointer
	@param content Element pointer
	@return void
*/
void clist_t__remove(CList_t *self, void *content)
{
	clist_block_t *block = NULL;
	clist_block_t *prev = NULL;
	
	block = self->clist.head;
	while (block)
	{
		if (block->content == content)
		{
			if (prev)
				prev->next = block->next;
			else
				self->clist.head = block->next;
			
			free(block);
			
			return;
		} else {
			prev = block;
			block = block->next;
		}
	}
}

/**
	@relates clist_s
	@fn void clist_t__empty(CList_t *self)
	@brief Empty CList
	@param self Object pointer
	@return void
*/
void clist_t__empty(CList_t *self)
{
	void *content = NULL;
	
	content = self->clist.pop(self);
	while (content)
		content = self->clist.pop(self);
}

/**
	@relates clist_s
	@fn void clist_t__entityUpdateAndDraw(CList_t *self, layer_t updateLayer, layer_t drawLayer)
	@brief Update and draw enities selected by layer
	@param self Object pointer
	@param updateLayer Layers to update
	@param drawLayer Layers to draw
	@return void
*/
void clist_t__entityUpdateAndDraw(CList_t *self, layer_t updateLayer, layer_t drawLayer)
{
	layer_t		  layer = NO_LAYER;
	Entity_t		 *content = NULL;
	clist_block_t	*block = NULL;
	
	content = self->clist.iter(self, &block);
	while (content)
	{
		layer = content->entity.getLayer(content);
		
		if (layer & updateLayer)
			content->entity.update(content);

		if (layer & drawLayer)
			content->entity.draw(content);
		
		content = self->clist.iter(self, &block);
	}
}

retno_t clist_t__ctor(CList_t *self)
{
	self->clist.head = NULL;
	self->clist.push = &clist_t__push;
	self->clist.pop = &clist_t__pop;
	self->clist.iter = &clist_t__iter;
	self->clist.remove = &clist_t__remove;
	self->clist.empty = &clist_t__empty;
	self->clist.entityUpdateAndDraw = &clist_t__entityUpdateAndDraw;
	
	return SUCCESS;
}

retno_t clist_t__dtor(CList_t *self)
{
	void *content = NULL;
	
	content = self->clist.pop(self);
	while (content)
	{
		delete(content);
		content = self->clist.pop(self);
	}
	
	return SUCCESS;
}
