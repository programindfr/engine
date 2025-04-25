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
	@file Entity.h
*/

#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <Base.h>
#include <layer.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <stdint.h>

#define SIGMOID(VAR) (1 / (1 + exp(-(VAR))))
#define MODULUS(XVAR, YVAR) (sqrt((XVAR) * (XVAR) + (YVAR) * (YVAR)))
#define MAX(XVAR, YVAR) ((XVAR) > (YVAR) ? (XVAR) : (YVAR))
#define MIN(XVAR, YVAR) ((XVAR) < (YVAR) ? (XVAR) : (YVAR))

/**
	@enum action
	@brief Entity action enum
*/
typedef enum action {
	NO_ACT	= 0x00,
	ACT_01	= 0x01,
	ACT_02	= 0x02,
	ACT_03	= 0x04,
	ACT_04	= 0x08,
	ACT_05	= 0x10,
	ACT_06	= 0x20,
	ACT_07	= 0x40,
	ACT_08	= 0x80
} action_t;

#define ENTITYSPEED 0.1
#define Entity(WINDOW, XPOS, YPOS, LAYER, RECT, PATH) new(ENTITY, WINDOW, (float) (XPOS), (float) (YPOS), LAYER, RECT, PATH)

typedef struct entity_position {
	float x;
	float y;
	layer_t layer;
} entity_position_t;

typedef struct entity_graphics {
	char *path;
	SDL_Texture *texture;
	float width;
	float height;
	SDL_Texture *shadow;
	float radius;
} entity_graphics_t;

typedef struct entity_health {
	SDL_FRect rect;
	float hp;
	float hpMax;
	float hit;
} entity_health_t;

typedef struct entity_delta {
	float x;
	float y;
	float s;
} entity_delta_t;

typedef struct entity_state {
	uint8_t id;
	CList_t *transition;
} entity_state_t;

typedef struct entity_transition {
	uint32_t type;
	int32_t sym;
	action_t action;
	entity_state_t *to;
} entity_transition_t;

#define ENTITY_CLASS \
QTree_t			  *qtree;\
Window_t			 *window;\
entity_delta_t	   delta;\
entity_state_t	   *state;\
entity_health_t	  health;\
entity_position_t	position;\
entity_graphics_t	graphics;\
\
void		   (*draw)(Entity_t *self);\
void		   (*setQTree)(Entity_t *self, QTree_t *qtree);\
void		   (*setDeltaPosition)(Entity_t *self, float dx, float dy);\
void		   (*setLighting)(Entity_t *self, float radius, SDL_Color color);\
void		   (*transition)(Entity_t *self, uint8_t from, uint32_t type, int32_t sym, action_t action, uint8_t to);\
uint8_t		(*update)(Entity_t *self);\
CList_t		*(*states)(Entity_t *self);\
layer_t		(*getLayer)(Entity_t *self);\
QTree_t		*(*getQTree)(Entity_t *self);\
SDL_FRect	  (*getHitbox)(Entity_t *self);\
SDL_FRect	  (*getTextureRect)(Entity_t *self);\
SDL_FRect	  (*getLightingRect)(Entity_t *self);\
SDL_FPoint	 (*getPosition)(Entity_t *self);\
SDL_Texture	*(*getTexture)(Entity_t *self);\
SDL_Texture	*(*getLighting)(Entity_t *self);\

typedef struct entity_s {
	BASE_CLASS
	ENTITY_CLASS
} entity_t;

union entity_u {
	type_t type;
	entity_t entity;
};

retno_t entity_t__ctor(Entity_t *self);
retno_t entity_t__dtor(Entity_t *self);

#endif/*__ENTITY_H__*/
