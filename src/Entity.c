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
	@file Entity.c
*/

#include <Base.h>
#include <CList.h>
#include <Entity.h>
#include <QTree.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdint.h>
#include <stdlib.h>
#include <Window.h>

/**
	@relates entity_s
	@fn void entity_t__draw(Entity_t *self)
	@brief Draw element on the screen
	@param self Object pointer
	@return void
*/
void entity_t__draw(Entity_t *self)
{
	self->entity.window->window.putOnCamera(self->entity.window, self);
}

/**
	@relates entity_s
	@fn CList_t *entity_t__states(Entity_t *self)
	@brief Fetch Entity automata states
	@param self Object pointer
	@return CList_t of entity_state_t pointers
*/
CList_t *entity_t__states(Entity_t *self)
{
	uint8_t				flag = 0;
	CList_t				*list = NULL;
	CList_t				*successor = NULL;
	clist_block_t		  *transblock = NULL;
	clist_block_t		  *stateblock = NULL;
	entity_state_t		 *elem = NULL;
	entity_state_t		 *state = NULL;
	entity_transition_t	*transition = NULL;
	
	list = CList();
	successor = CList();
	
	elem = self->entity.state;
	list->clist.push(list, elem);
	successor->clist.push(successor, elem);
	
	elem = successor->clist.pop(successor);
	while (elem)
	{
		transblock = NULL;
		transition = elem->transition->clist.iter(elem->transition, &transblock);
		
		while (transition)
		{
			flag = 0;
			stateblock = NULL;
			state = list->clist.iter(list, &stateblock);
			
			while (state)
			{
				if (state == transition->to)
					flag = 1;
				
				state = list->clist.iter(list, &stateblock);
			}
			
			if (!flag)
			{
				list->clist.push(list, transition->to);
				successor->clist.push(successor, transition->to);
			}
			
			transition = elem->transition->clist.iter(elem->transition, &transblock);
		}
		
		elem = successor->clist.pop(successor);
	}
	
	delete(successor);
	
	return list;
}

/**
	@relates entity_s
	@fn void entity_t__transition(Entity_t *self, uint8_t from, uint32_t type, int32_t sym, action_t action, uint8_t to)
	@brief Add a new transition in the Entity automata
	@param self Object pointer
	@param self Source state id
	@param self Symbol type
	@param self Symbol sym
	@param self Transition effect
	@param self Destination state id
	@return void
*/
void entity_t__transition(Entity_t *self, uint8_t from, uint32_t type, int32_t sym, action_t action, uint8_t to)
{
	CList_t				*states = NULL;
	clist_block_t		  *block = NULL;
	entity_state_t		 *elem = NULL;
	entity_state_t		 *ptrto = NULL;
	entity_transition_t	*transition = NULL;
	
	transition = calloc(1, sizeof(entity_transition_t));
	transition->type = type;
	transition->sym = sym;
	transition->action = action;
	
	states = self->entity.states(self);
	elem = states->clist.iter(states, &block);
	
	while (elem)
	{
		if (elem->id == to)
		{
			ptrto = elem;
			elem = NULL;
		} else
			elem = states->clist.iter(states, &block);
	}
	
	if (!ptrto)
	{
		ptrto = calloc(1, sizeof(entity_state_t));
		ptrto->id = to;
		ptrto->transition = CList();
	}
	
	transition->to = ptrto;
	
	elem = states->clist.pop(states);
	while(elem)
	{
		if (elem->id == from)
			elem->transition->clist.push(elem->transition, transition);
		
		elem = states->clist.pop(states);
	}
	
	delete(states);
}

/**
	@relates entity_s
	@fn uint8_t entity_t__update(Entity_t *self)
	@brief Update Entity internal automata
	@param self Object pointer
	@return Current state id of the Entity automata
*/
uint8_t entity_t__update(Entity_t *self)
{
	uint8_t				flag = 0;
	CList_t				*list = NULL;
	QTree_t				*qtree = NULL;
	Entity_t			   *elem = NULL;
	uint64_t			   deltatime = 0;
	Window_t			   *window = NULL;
	action_t			   action = NO_ACT;
	SDL_FRect			  area;
	SDL_FRect			  rect;
	SDL_Event			  event;
	SDL_FRect			  elemrect;
	clist_block_t		  *block = NULL;
	entity_state_t		 *state = NULL;
	entity_transition_t	*transition = NULL;

	window = self->entity.window;
	event = window->window.getEvent(window);
	state = self->entity.state;
	deltatime = window->window.getDeltatime(window);
	rect = self->entity.getHitbox(self);
	area = self->entity.getTextureRect(self);
	area.x -= area.w;
	area.y -= area.h;
	area.w *= 3;
	area.h *= 3;
	
	transition = state->transition->clist.iter(state->transition, &block);
	while (transition)
	{
		if (transition->type == event.type &&
			transition->sym == event.key.keysym.sym)
		{
			action = transition->action;
			self->entity.state = transition->to;
			transition = NULL;
		} else
			transition = state->transition->clist.iter(state->transition, &block);
	}
	
	if (action & ACT_01)
		self->entity.delta.x = ENTITYSPEED;
	
	if (action & ACT_02)
		self->entity.delta.x = -ENTITYSPEED;
	
	if (action & ACT_03)
		self->entity.delta.x = 0.0;
	
	if (action & ACT_04)
		self->entity.delta.y = ENTITYSPEED;
	
	if (action & ACT_05)
		self->entity.delta.y = -ENTITYSPEED;
	
	if (action & ACT_06)
		self->entity.delta.y = 0.0;

	if (action & ACT_07)
		self->entity.delta.s = 2.0;
	
	if (action & ACT_08)
		self->entity.delta.s = 1.0;

	rect.x += self->entity.delta.x * self->entity.delta.s * deltatime;
	rect.y += self->entity.delta.y * self->entity.delta.s * deltatime;
	area.x += self->entity.delta.x * self->entity.delta.s * deltatime;
	area.y += self->entity.delta.y * self->entity.delta.s * deltatime;
	qtree = self->entity.getQTree(self);

	if (qtree)
		list = qtree->qtree.fetch(qtree, area);

	if (list)
	{
		elem = list->clist.pop(list);
		while (elem)
		{
			if (elem != self &&
				elem->entity.getLayer(elem) & self->entity.getLayer(self))
			{
				elemrect = elem->entity.getHitbox(elem);
				flag |= SDL_HasIntersectionF(&elemrect, &rect);
			}
			
			elem = list->clist.pop(list);
		}
		
		delete(list);
	}

	if (!flag)
		self->entity.setDeltaPosition(
			self,
			self->entity.delta.x * self->entity.delta.s * deltatime,
			self->entity.delta.y * self->entity.delta.s * deltatime
		);
	
	return self->entity.state->id;
}

/**
	@relates entity_s
	@fn QTree_t *entity_t__getQTree(Entity_t *self)
	@brief Get entity qtree
	@param self Object pointer
	@return Entity qtree
*/
QTree_t *entity_t__getQTree(Entity_t *self)
{
	return self->entity.qtree;
}

/**
	@relates entity_s
	@fn void entity_t__setQTree(Entity_t *self, QTree_t *qtree)
	@brief Set entity qtree
	@param self Object pointer
	@param qtree New qtree pointer
	@return void
*/
void entity_t__setQTree(Entity_t *self, QTree_t *qtree)
{
	self->entity.qtree = qtree;
}

/**
	@relates entity_s
	@fn SDL_FPoint entity_t__getPosition(Entity_t *self)
	@brief Get entity position
	@param self Object pointer
	@return Entity position
*/
SDL_FPoint entity_t__getPosition(Entity_t *self)
{
	SDL_FPoint point;
	
	point.x = self->entity.position.x;
	point.y = self->entity.position.y;
	
	return point;
}

/**
	@relates entity_s
	@fn void entity_t__setDeltaPosition(Entity_t *self, float dx, float dy)
	@brief Set entity delta position
	@param self Object pointer
	@param dx Delta x position
	@param dy Delta y position
	@return void
*/
void entity_t__setDeltaPosition(Entity_t *self, float dx, float dy)
{
	self->entity.position.x += dx;
	self->entity.position.y += dy;
}

/**
	@relates entity_s
	@fn layer_t entity_t__getLayer(Entity_t *self)
	@brief Get entity layer
	@param self Object pointer
	@return Entity layer
*/
layer_t entity_t__getLayer(Entity_t *self)
{
	return self->entity.position.layer;
}

/**
	@relates entity_s
	@fn SDL_FRect entity_t__getTextureRect(Entity_t *self)
	@brief Get entity texture rect
	@param self Object pointer
	@return Entity texture rect
*/
SDL_FRect entity_t__getTextureRect(Entity_t *self)
{
	SDL_FRect rect;

	rect.x = self->entity.position.x;
	rect.y = self->entity.position.y;
	rect.w = self->entity.graphics.width;
	rect.h = self->entity.graphics.height;

	return rect;
}

/**
	@relates entity_s
	@fn SDL_Texture *entity_t__getTexture(Entity_t *self)
	@brief Get entity texture
	@param self Object pointer
	@return Entity texture
*/
SDL_Texture *entity_t__getTexture(Entity_t *self)
{
	return self->entity.graphics.texture;
}

/**
	@relates entity_s
	@fn SDL_FRect entity_t__getHitbox(Entity_t *self)
	@brief Get entity hitbox rect
	@param self Object pointer
	@return Entity hitbox rect
*/
SDL_FRect entity_t__getHitbox(Entity_t *self)
{
	SDL_FRect rect;

	rect = self->entity.health.rect;
	rect.x += self->entity.position.x;
	rect.y += self->entity.position.y;

	return rect;
}

/**
	@relates entity_s
	@fn void entity_t__setLighting(Entity_t *self, float radius, SDL_Color color)
	@brief Set entity lighting radius/color by processing texture
	@param self Object pointer
	@param radius Lighting radius
	@param color Lighting color
	@return void

	@warning This function is slow so don't call it often
*/
void entity_t__setLighting(Entity_t *self, float radius, SDL_Color color)
{
	int				i, j;
	int				width;
	int				height;
	int				brightness;
	float			  lenght;
	float			  lenghtmod;
	float			  range;
	uint32_t		   *pixels = NULL;
	SDL_FRect		  rect;
	SDL_PixelFormat	*format = NULL;

	self->entity.graphics.radius = radius;
	rect = self->entity.getTextureRect(self);
	width = rect.w * radius;
	height = rect.h * radius;
	lenght = MIN(rect.w, rect.h) * radius / 2;
	lenghtmod = MODULUS(lenght, lenght);

	pixels = calloc(width * height, sizeof(uint32_t));
	format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
	
	if (self->entity.graphics.shadow)
		SDL_DestroyTexture(self->entity.graphics.shadow);

	self->entity.graphics.shadow = SDL_CreateTexture(
		self->entity.window->window.renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		width,
		height
	);
	SDL_SetTextureBlendMode(self->entity.graphics.shadow, SDL_BLENDMODE_ADD);

	for (i = 0; i < width; ++i)
	{
		for (j = 0; j < height; ++j)
		{
			range = (lenghtmod - MODULUS(i - lenght, j - lenght)) * 12 / lenghtmod - 8;
			brightness = color.a * SIGMOID(range);
			pixels[i * width + j] = SDL_MapRGBA(format, color.r, color.g, color.b, brightness);
		}
	}

	SDL_UpdateTexture(self->entity.graphics.shadow, NULL, pixels, sizeof(uint32_t) * width);
	
	SDL_FreeFormat(format);
	free(pixels);
}

/**
	@relates entity_s
	@fn SDL_Texture *entity_t__getLighting(Entity_t *self)
	@brief Get entity lighting texture
	@param self Object pointer
	@return Entity lighting texture
*/
SDL_Texture *entity_t__getLighting(Entity_t *self)
{
	return self->entity.graphics.shadow;
}

/**
	@relates entity_s
	@fn SDL_FRect entity_t__getLightingRect(Entity_t *self)
	@brief Get entity Lighting rect
	@param self Object pointer
	@return Entity lighting rect
*/
SDL_FRect entity_t__getLightingRect(Entity_t *self)
{
	SDL_FRect rect;

	rect = self->entity.getTextureRect(self);
	rect.x -= rect.w * (self->entity.graphics.radius - 1) / 2;
	rect.y -= rect.h * (self->entity.graphics.radius - 1) / 2;
	rect.w *= self->entity.graphics.radius;
	rect.h *= self->entity.graphics.radius;

	return rect;
}

retno_t entity_t__ctor(Entity_t *self)
{
	int retno;
	int width;
	int height;
	
	self->entity.graphics.texture = IMG_LoadTexture(
		self->entity.window->window.renderer,
		self->entity.graphics.path
	);
	LOG_ERROR(!self->entity.graphics.texture, SDL_GetError());
	
	retno = SDL_QueryTexture(
		self->entity.graphics.texture,
		NULL, NULL,
		&width,
		&height
	);
	LOG_ERROR(retno, SDL_GetError());
	self->entity.graphics.width = width;
	self->entity.graphics.height = height;
	
	self->entity.delta.s = 1.0;
	
	self->entity.state = calloc(1, sizeof(entity_state_t));
	self->entity.state->id = 0;
	self->entity.state->transition = CList();
	
	self->entity.draw				= &entity_t__draw;
	self->entity.states			  = &entity_t__states;
	self->entity.update			  = &entity_t__update;
	self->entity.getQTree			= entity_t__getQTree;
	self->entity.setQTree			= entity_t__setQTree;
	self->entity.getLayer			= entity_t__getLayer;
	self->entity.getHitbox		   = &entity_t__getHitbox;
	self->entity.getTexture		  = &entity_t__getTexture;
	self->entity.transition		  = &entity_t__transition;
	self->entity.setLighting		 = &entity_t__setLighting;
	self->entity.getLighting		 = &entity_t__getLighting;
	self->entity.getPosition		 = &entity_t__getPosition;
	self->entity.getTextureRect	  = &entity_t__getTextureRect;
	self->entity.getLightingRect	 = &entity_t__getLightingRect;
	self->entity.setDeltaPosition	= &entity_t__setDeltaPosition;
	
	
	return SUCCESS;
}

retno_t entity_t__dtor(Entity_t *self)
{
	CList_t				*states = NULL;
	clist_block_t		  *block = NULL;
	entity_state_t		 *elem = NULL;
	entity_transition_t	*transition = NULL;
	
	if (self->entity.graphics.texture)
		SDL_DestroyTexture(self->entity.graphics.texture);

	if (self->entity.graphics.shadow)
		SDL_DestroyTexture(self->entity.graphics.shadow);
	
	if (self->entity.state)
	{
		states = self->entity.states(self);
		elem = states->clist.iter(states, &block);
		
		while (elem)
		{
			transition = elem->transition->clist.pop(elem->transition);
			while (transition)
			{
				free(transition);
				transition = elem->transition->clist.pop(elem->transition);
			}
			
			delete(elem->transition);
			elem = states->clist.iter(states, &block);
		}
		
		elem = states->clist.pop(states);
		while (elem)
		{
			free(elem);
			elem = states->clist.pop(states);
		}
		
		delete(states);
	}
	
	return SUCCESS;
}
