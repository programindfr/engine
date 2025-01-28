#ifndef __ENGINE_H__
#define __ENGINE_H__

/*
INCLUDE SECTION
*/

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdarg.h>
#include <stdlib.h>

/*
CLASS STRUCTURE SECTION
*/

/*
Class structure type
*/
typedef union clist_u CList_t;
typedef union qtree_u QTree_t;
typedef union window_u Window_t;
typedef union entity_u Entity_t;

/*
Class ctor/dtor return state
*/
typedef enum errno {
	SUCCESS,
	FAILURE
} errno_t;

/*
Class type enum
*/
typedef enum type {
	CLIST	= 0x1,
	QTREE	= 0x2,
	WINDOW	= 0x4,
	ENTITY	= 0x8
} type_t;

/*
Entity layer enum
*/
typedef enum layer {
	NO_LAYER	= 0x0000,
	LAYER_01	= 0x0001,
	LAYER_02	= 0x0002,
	LAYER_03	= 0x0004,
	LAYER_04	= 0x0008,
	LAYER_05	= 0x0010,
	LAYER_06	= 0x0020,
	LAYER_07	= 0x0040,
	LAYER_08	= 0x0080,
	LAYER_09	= 0x0100,
	LAYER_10	= 0x0200,
	LAYER_11	= 0x0400,
	LAYER_12	= 0x0800,
	LAYER_13	= 0x1000,
	LAYER_14	= 0x2000,
	LAYER_15	= 0x4000,
	LAYER_16	= 0x8000
} layer_t;

/*
Entity action enum
*/
typedef enum action {
	NO_ACT	= 0x0,
	ACT_01	= 0x1,
	ACT_02	= 0x2,
	ACT_03	= 0x4,
	ACT_04	= 0x8
} action_t;

/*
Function definition
*/
void *new(type_t type, ...);
void delete(void *ptr);

/*
BASE_CLASS
*/
#define BASE_CLASS \
type_t type;

/*
CLIST_CLASS
*/
#define CList() new(CLIST)

typedef struct clist_block {
	void *content;
	struct clist_block *next;
} clist_block_t;

#define CLIST_CLASS \
clist_block_t *head;\
void (*push)(CList_t *self, void *content);\
void *(*pop)(CList_t *self);\
void *(*iter)(CList_t *self, clist_block_t **block);\
void (*remove)(CList_t *self, void *content);

typedef struct clist_s {
	BASE_CLASS
	CLIST_CLASS
} clist_t;

union clist_u {
	type_t type;
	clist_t clist;
};

/*
QTREE_CLASS
*/
#define QTree(RECT) new(QTREE, RECT)

#define QTREE_CLASS \
SDL_FRect rect;\
Entity_t *content[4];\
QTree_t *tree[4];\
void (*insert)(QTree_t *self, Entity_t *content);\
uint8_t (*remove)(QTree_t *self, Entity_t *content);\
CList_t *(*fetch)(QTree_t *self, SDL_FRect rect);\
void (*update)(QTree_t *self);

typedef struct qtree_s {
	BASE_CLASS
	QTREE_CLASS
} qtree_t;

union qtree_u {
	type_t type;
	qtree_t qtree;
};

/*
WINDOW_CLASS
*/
#define Window() new(WINDOW)

typedef struct window_camera {
	SDL_FRect rect;
	SDL_Texture *texture;
	SDL_Texture *shadow;
	float lighting;
} window_camera_t;

#define WINDOW_CLASS \
SDL_Window *window;\
SDL_Renderer *renderer;\
SDL_Event event;\
uint64_t time;\
uint64_t deltatime;\
window_camera_t camera;\
void (*put)(Window_t *self, Entity_t *content);\
void (*update)(Window_t *self);

typedef struct window_s {
	BASE_CLASS
	WINDOW_CLASS
} window_t;

union window_u {
	type_t type;
	window_t window;
};

/*
ENTITY_CLASS
*/
#define Entity(EWINDOW, RECT, LAYER, PATH) new(ENTITY, EWINDOW, RECT, LAYER, PATH)

typedef struct entity_graphics {
	char *path;
	SDL_Texture *texture;
	SDL_Color lightingColor;
	float lightingRadius;
} entity_graphics_t;

typedef struct entity_delta {
	float x;
	float y;
	float l;
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
Window_t *window;\
SDL_FRect rect;\
layer_t layer;\
entity_graphics_t graphics;\
entity_delta_t delta;\
entity_state_t *state;\
void (*draw)(Entity_t *self);\
CList_t *(*states)(Entity_t *self);\
void (*transition)(Entity_t *self, uint8_t from, uint32_t type, int32_t sym, action_t action, uint8_t to);

typedef struct entity_s {
	BASE_CLASS
	ENTITY_CLASS
} entity_t;

union entity_u {
	type_t type;
	entity_t entity;
};

/*
Class_t union
*/
typedef union class {
	type_t type;
	clist_t clist;
	qtree_t qtree;
	window_t window;
	entity_t entity;
} Class_t;

/*
CLASS FUNCTION SECTION
*/

void clist_t__push(CList_t *self, void *content)
{
	clist_block_t *block = NULL;
	
	block = calloc(1, sizeof(clist_block_t));
	
	block->content = content;
	block->next = self->clist.head;
	
	self->clist.head = block;
}

void *clist_t__pop(CList_t *self)
{
	void			*content = NULL;
	clist_block_t	*block = NULL;
	
	if (!self->clist.head)
		return content;
	
	block = self->clist.head;
	content = block->content;
	self->clist.head = block->next;
	
	free(block);
	
	return content;
}

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

void qtree_t__insert(QTree_t *self, Entity_t *content)
{
	size_t		i, j;
	size_t		bound;
	size_t		flag = (size_t) NULL;
	Entity_t	*elem = NULL;
	SDL_FRect	rect;
	SDL_FPoint	point;
	
	point.x = content->entity.rect.x;
	point.y = content->entity.rect.y;
	
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
			elem = self->qtree.content[i];
		
		point.x = elem->entity.rect.x;
		point.y = elem->entity.rect.y;
		
		for (j = 0; j < 4; ++j)
		{
			rect.x = self->qtree.rect.x + (j % 2) * rect.w;
			rect.y = self->qtree.rect.y + (j / 2) * rect.h;
			
			if (SDL_PointInFRect(&point, &rect))
			{
				if (!self->qtree.tree[i])
					self->qtree.tree[i] = QTree(rect);
				
				self->qtree.tree[i]->qtree.insert(self->qtree.tree[i], elem);
				break;
			}
		}
	}
}

uint8_t qtree_t__remove(QTree_t *self, Entity_t *content)
{
	size_t		i;
	SDL_FPoint	point;
	
	point.x = content->entity.rect.x;
	point.y = content->entity.rect.y;
	
	if (!SDL_PointInFRect(&point, &(self->qtree.rect)))
		return 0;
	
	for (i = 0; i < 4; ++i)
	{
		if (self->qtree.content[i] == content)
		{
			self->qtree.content[i] = NULL;
			return 1;
		}
		
		if (self->qtree.tree[i])
		{
			if (self->qtree.tree[i]->qtree.remove(self->qtree.tree[i], content))
				return 1;
		}
	}
	
	return 0;
}

CList_t *qtree_t__fetch(QTree_t *self, SDL_FRect rect)
{
	size_t		i;
	CList_t		*list = NULL;
	CList_t		*sublist = NULL;
	Entity_t	*elem = NULL;
	SDL_FRect	subrect;
	SDL_FPoint	point;
	
	list = CList();
	
	for (i = 0; i < 4; ++i)
	{
		if (self->qtree.content[i])
		{
			point.x = self->qtree.content[i]->entity.rect.x;
			point.y = self->qtree.content[i]->entity.rect.y;
			
			if (SDL_PointInFRect(&point, &rect))
				list->clist.push(list, self->qtree.content[i]);
		}
		
		if (self->qtree.tree[i])
		{
			subrect = self->qtree.tree[i]->qtree.rect;
			
			if (SDL_HasIntersectionF(&subrect, &rect))
			{
				sublist = self->qtree.tree[i]->qtree.fetch(self->qtree.tree[i], rect);
				
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

void qtree_t__update(QTree_t *self)
{
	size_t		i, j;
	size_t		count = 0;
	size_t		flag = (size_t) NULL;
	CList_t		*elemlist = NULL;
	CList_t		*qtreelist = NULL;
	QTree_t		*subqtree = NULL;
	Entity_t	*elem = NULL;
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
				point.x = subqtree->qtree.content[i]->entity.rect.x;
				point.y = subqtree->qtree.content[i]->entity.rect.y;
				
				if (!SDL_PointInFRect(&point, &(subqtree->qtree.rect)))
				{
					elemlist->clist.push(elemlist, subqtree->qtree.content[i]);
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

void window_t__put(Window_t *self, Entity_t *content)
{
	SDL_SetRenderTarget(self->window.renderer, self->window.camera.texture);
	SDL_RenderCopyF(
		self->window.renderer,
		content->entity.graphics.texture,
		NULL,
		&(content->entity.rect)
	);
}

void window_t__update(Window_t *self)
{
	uint64_t time;
	
	time = SDL_GetTicks64();
	self->window.deltatime = time - self->window.time;
	self->window.time = time;
	
	if (!SDL_PollEvent(&(self->window.event)))
		self->window.event.type = SDL_FIRSTEVENT;
	
	SDL_SetRenderTarget(self->window.renderer, NULL);
	SDL_RenderCopyF(self->window.renderer, self->window.camera.texture, NULL, NULL);
	SDL_RenderPresent(self->window.renderer);
	
	SDL_SetRenderTarget(self->window.renderer, self->window.camera.texture);
	SDL_SetRenderDrawColor(self->window.renderer, 0, 0, 0, 0);
	SDL_RenderClear(self->window.renderer);
}

void entity_t__draw(Entity_t *self)
{
	self->entity.window->window.put(self->entity.window, self);
}

CList_t *entity_t__states(Entity_t *self)
{
	uint8_t				flag = 0;
	CList_t				*list = NULL;
	CList_t				*successor = NULL;
	clist_block_t		*transblock = NULL;
	clist_block_t		*stateblock = NULL;
	entity_state_t		*elem = NULL;
	entity_state_t		*state = NULL;
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

void entity_t__transition(Entity_t *self, uint8_t from, uint32_t type, int32_t sym, action_t action, uint8_t to)
{
	CList_t				*states = NULL;
	clist_block_t		*block = NULL;
	entity_state_t		*elem = NULL;
	entity_state_t		*ptrto = NULL;
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
		
		elem = states->clist.iter(states, &block);
	}
	
	delete(states);
}

/*
CONSTRUCTOR SECTION
*/

errno_t clist_t__ctor(CList_t *self)
{
	self->clist.head = NULL;
	self->clist.push = &clist_t__push;
	self->clist.pop = &clist_t__pop;
	self->clist.iter = &clist_t__iter;
	self->clist.remove = &clist_t__remove;
	
	return SUCCESS;
}

errno_t qtree_t__ctor(QTree_t *self)
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
	
	return SUCCESS;
}

errno_t window_t__ctor(Window_t *self)
{
	if (SDL_Init(SDL_INIT_VIDEO))
		return FAILURE;
	
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
		return FAILURE;
	
	if (
		SDL_CreateWindowAndRenderer(
			800, 600,
			SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE,
			&(self->window.window),
			&(self->window.renderer)
		)
	)
		return FAILURE;
	
	self->window.event.type = SDL_FIRSTEVENT;
	self->window.time = 0;
	self->window.deltatime = 0;
	
	self->window.camera.rect.x = 0.0;
	self->window.camera.rect.y = 0.0;
	self->window.camera.rect.w = 800.0;
	self->window.camera.rect.h = 600.0;
	
	self->window.camera.texture = SDL_CreateTexture(
		self->window.renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		self->window.camera.rect.w,
		self->window.camera.rect.h
	);
	
	self->window.camera.shadow = SDL_CreateTexture(
		self->window.renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		self->window.camera.rect.w,
		self->window.camera.rect.h
	);
	
	self->window.camera.lighting = 1.0;
	
	self->window.put = &window_t__put;
	self->window.update = &window_t__update;
	
	return SUCCESS;
}

errno_t entity_t__ctor(Entity_t *self)
{
	self->entity.graphics.texture = IMG_LoadTexture(
		self->entity.window->window.renderer,
		self->entity.graphics.path
	);
	self->entity.graphics.lightingColor.r = 0x00;
	self->entity.graphics.lightingColor.g = 0x00;
	self->entity.graphics.lightingColor.b = 0x00;
	self->entity.graphics.lightingColor.a = 0x00;
	self->entity.graphics.lightingRadius = 0.0;
	
	self->entity.state = calloc(1, sizeof(entity_state_t));
	self->entity.state->id = 0;
	self->entity.state->transition = CList();
	
	self->entity.draw = &entity_t__draw;
	self->entity.states = &entity_t__states;
	self->entity.transition = &entity_t__transition;
	
	return SUCCESS;
}

/*
DESTRUCTOR SECTION
*/

errno_t clist_t__dtor(CList_t *self)
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

errno_t qtree_t__dtor(QTree_t *self)
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

errno_t window_t__dtor(Window_t *self)
{
	if (self->window.camera.texture)
		SDL_DestroyTexture(self->window.camera.texture);
	
	if (self->window.camera.shadow)
		SDL_DestroyTexture(self->window.camera.shadow);
	
	if (self->window.renderer)
		SDL_DestroyRenderer(self->window.renderer);
	
	if (self->window.window)
		SDL_DestroyWindow(self->window.window);
	
	SDL_Quit();
	
	return SUCCESS;
}

errno_t entity_t__dtor(Entity_t *self)
{
	if (self->entity.graphics.texture)
		SDL_DestroyTexture(self->entity.graphics.texture);
	
	if (self->entity.state)
		return FAILURE;
	
	return SUCCESS;
}

/*
NEW ALLOCATOR SECTION
*/

void *new(type_t type, ...)
{
	void	*self = NULL;
	va_list	arguments;
	
	va_start(arguments, type);
	
	if (type & CLIST)
	{
		self = calloc(1, sizeof(CList_t));
		clist_t__ctor((CList_t *) self);
	}
	
	if (type & QTREE)
	{
		self = calloc(1, sizeof(QTree_t));
		((QTree_t *) self)->qtree.rect = va_arg(arguments, SDL_FRect);
		qtree_t__ctor((QTree_t *) self);
	}
	
	if (type & WINDOW)
	{
		self = calloc(1, sizeof(Window_t));
		window_t__ctor((Window_t *) self);
	}
	
	if (type & ENTITY)
	{
		self = calloc(1, sizeof(Entity_t));
		((Entity_t *) self)->entity.window = va_arg(arguments, Window_t *);
		((Entity_t *) self)->entity.rect = va_arg(arguments, SDL_FRect);
		((Entity_t *) self)->entity.layer = va_arg(arguments, layer_t);
		((Entity_t *) self)->entity.graphics.path = va_arg(arguments, char *);
		entity_t__ctor((Entity_t *) self);
	}
	
	va_end(arguments);
	*(type_t *) self = type;
	
	return self;
}

/*
DELETE DEALLOCATOR SECTION
*/

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

#endif/*__ENGINE_H__*/
