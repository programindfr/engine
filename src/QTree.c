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
	@file QTree.c
*/

#include <Base.h>
#include <CList.h>
#include <Entity.h>
#include <QTree.h>
#include <SDL2/SDL.h>
#include <stddef.h>
#include <stdint.h>
#include <Window.h>

/**
	@relates qtree_s
	@fn void qtree_t__insert(QTree_t *self, Entity_t *content)
	@brief Insert element
	@param self Object pointer
	@param content Element pointer
	@return void
*/
void qtree_t__insert(QTree_t *self, Entity_t *content)
{
	size_t		i, j;
	size_t		bound;
	size_t		flag = (size_t) NULL;
	QTree_t	   *qtree = NULL;
	Entity_t	  *elem = NULL;
	SDL_FRect	 rect;
	SDL_FPoint	point;

	if (!content->entity.getQTree(content))
		content->entity.setQTree(content, self);
	
	point = content->entity.getPosition(content);
	
	for (i = 0; i < 4; ++i)
		flag |= (size_t) self->qtree.tree[i];
	
	bound = flag ? 0 : 4;
	for (i = 0; i < bound; ++i)
	{
		if (!self->qtree.content[i])
		{
			self->qtree.content[i] = content;
			return;
		}
	}
	
	bound++;
	rect.w = self->qtree.rect.w / 2;
	rect.h = self->qtree.rect.h / 2;
	
	for (i = 0; i < bound; ++i)
	{
		elem = content;
		if (!flag && i < 4)
		{
			elem = self->qtree.content[i];
			self->qtree.content[i] = NULL;
		}
		
		point = elem->entity.getPosition(elem);
		
		for (j = 0; j < 4; ++j)
		{
			rect.x = self->qtree.rect.x + (j % 2) * rect.w;
			rect.y = self->qtree.rect.y + (j / 2) * rect.h;
			
			if (SDL_PointInFRect(&point, &rect))
			{
				if (!self->qtree.tree[j])
					self->qtree.tree[j] = QTree(rect);

				qtree = self->qtree.tree[j];
				qtree->qtree.insert(qtree, elem);
				break;
			}
		}
	}
}

/**
	@relates qtree_s
	@fn uint8_t qtree_t__remove(QTree_t *self, Entity_t *content)
	@brief Remove element
	@param self Object pointer
	@param content Element pointer
	@return Boolean for recurrent algorithm
*/
uint8_t qtree_t__remove(QTree_t *self, Entity_t *content)
{
	size_t		i;
	QTree_t	   *qtree = NULL;
	SDL_FPoint	point;
	
	point = content->entity.getPosition(content);
	
	if (!SDL_PointInFRect(&point, &(self->qtree.rect)))
		return 0;
	
	for (i = 0; i < 4; ++i)
	{
		if (self->qtree.content[i] == content)
		{
			self->qtree.content[i] = NULL;
			content->entity.setQTree(content, NULL);
			return 1;
		}
		
		if (self->qtree.tree[i])
		{
			qtree = self->qtree.tree[i];
			
			if (qtree->qtree.remove(qtree, content))
				return 1;
		}
	}
	
	return 0;
}

/**
	@relates qtree_s
	@fn CList_t *qtree_t__fetch(QTree_t *self, SDL_FRect rect)
	@brief Fetch elements in rect area
	@param self Object pointer
	@param rect Area
	@return Chained list of elements
*/
CList_t *qtree_t__fetch(QTree_t *self, SDL_FRect rect)
{
	size_t	   i;
	CList_t	  *list = NULL;
	CList_t	  *sublist = NULL;
	QTree_t	  *qtree = NULL;
	Entity_t	 *elem = NULL;
	SDL_FRect	subrect;
	
	list = CList();
	
	for (i = 0; i < 4; ++i)
	{
		if (self->qtree.content[i])
		{
			elem = self->qtree.content[i];
			subrect = elem->entity.getHitbox(elem);

			if (SDL_HasIntersectionF(&subrect, &rect))
				list->clist.push(list, elem);
		}
		
		if (self->qtree.tree[i])
		{
			qtree = self->qtree.tree[i];
			subrect = qtree->qtree.rect;

			if (SDL_HasIntersectionF(&subrect, &rect))
			{
				sublist = qtree->qtree.fetch(qtree, rect);
				
				elem = sublist->clist.pop(sublist);
				while (elem)
				{
					list->clist.push(list, elem);
					elem = sublist->clist.pop(sublist);
				}
				
				delete(sublist);
			}
		}
	}
	
	return list;
}

/**
	@relates qtree_s
	@fn void qtree_t__update(QTree_t *self)
	@brief Update elements position in quadtree and clean unused quadtree
	@param self Object pointer
	@return void
*/
void qtree_t__update(QTree_t *self)
{
	size_t		i, j;
	size_t		count = 0;
	size_t		flag = (size_t) NULL;
	CList_t	   *elemlist = NULL;
	CList_t	   *qtreelist = NULL;
	QTree_t	   *subqtree = NULL;
	Entity_t	  *elem = NULL;
	SDL_FPoint	point;
	
	elemlist = CList();
	qtreelist = CList();
	qtreelist->clist.push(qtreelist, self);
	
	subqtree = qtreelist->clist.pop(qtreelist);
	while (subqtree)
	{
		count = 0;
		flag = (size_t) NULL;
		
		for (i = 0; i < 4; ++i)
		{
			if (subqtree->qtree.content[i])
			{
				elem = subqtree->qtree.content[i];
				point = elem->entity.getPosition(elem);
				
				if (!SDL_PointInFRect(&point, &(subqtree->qtree.rect)))
				{
					elemlist->clist.push(elemlist, elem);
					subqtree->qtree.content[i] = NULL;
				}
			}
			
			if (subqtree->qtree.tree[i])
			{
				for (j = 0; j < 4; ++j)
				{
					if (subqtree->qtree.tree[i]->qtree.content[j])
						count++;
					
					flag |= (size_t) subqtree->qtree.tree[i]->qtree.tree[j];
				}
			}
		}
		
		for (i = 0; i < 4; ++i)
		{
			if (subqtree->qtree.tree[i])
			{
				if (count < 5 && !flag)
				{
					for (j = 0; j < 4; ++j)
					{
						elem = subqtree->qtree.tree[i]->qtree.content[j];
						
						if (elem)
						{
							elemlist->clist.push(elemlist, elem);
							subqtree->qtree.tree[i]->qtree.content[j] = NULL;
						}
					}
					
					delete(subqtree->qtree.tree[i]);
					subqtree->qtree.tree[i] = NULL;
				} else
					qtreelist->clist.push(qtreelist, subqtree->qtree.tree[i]);
			}
		}
		
		subqtree = qtreelist->clist.pop(qtreelist);
	}
	
	elem = elemlist->clist.pop(elemlist);
	while (elem)
	{
		self->qtree.insert(self, elem);
		elem = elemlist->clist.pop(elemlist);
	}
	
	delete(qtreelist);
	delete(elemlist);
}

/**
	@relates qtree_s
	@fn void qtree_t__draw(QTree_t *self, Window_t *window)
	@brief Draw quadtree area
	@param self Object pointer
	@param window Window pointer
	@return void
	
	@warning Use it only for debugging
*/
void qtree_t__draw(QTree_t *self, Window_t *window)
{
	size_t i;
	
	SDL_SetRenderTarget(window->window.renderer, window->window.camera.texture);
	SDL_SetRenderDrawColor(window->window.renderer, 255, 0, 0, 255);
	SDL_RenderDrawRectF(window->window.renderer, &(self->qtree.rect));
	
	for (i = 0; i < 4; ++i)
	{
		if (self->qtree.tree[i])
			self->qtree.tree[i]->qtree.draw(self->qtree.tree[i], window);
	}
}

retno_t qtree_t__ctor(QTree_t *self)
{
	size_t i;
	
	for (i = 0; i < 4; ++i)
	{
		self->qtree.content[i] = NULL;
		self->qtree.tree[i] = NULL;
	}
	
	self->qtree.insert = &qtree_t__insert;
	self->qtree.remove = &qtree_t__remove;
	self->qtree.fetch = &qtree_t__fetch;
	self->qtree.update = &qtree_t__update;
	self->qtree.draw = &qtree_t__draw;
	
	return SUCCESS;
}

retno_t qtree_t__dtor(QTree_t *self)
{
	size_t i;
	
	for (i = 0; i < 4; ++i)
	{
		if (self->qtree.content[i])
			delete(self->qtree.content[i]);
		
		if (self->qtree.tree[i])
			delete(self->qtree.tree[i]);
	}
	
	return SUCCESS;
}
