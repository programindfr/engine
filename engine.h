/**
	@file engine.h
*/

#ifndef __ENGINE_H__
#define __ENGINE_H__

/*
INCLUDE SECTION
*/

#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdarg.h>
#include <stdlib.h>

/*
CLASS STRUCTURE SECTION
*/

#define LOG_ERROR(RETNO, NAME) if (RETNO) SDL_LogError(SDL_LOG_CATEGORY_ERROR, "l:%d no:%d %s", __LINE__, RETNO, NAME)
#define SIGMOID(VAR) (1 / (1 + exp(-(VAR))))
#define MODULUS(XVAR, YVAR) (sqrt((XVAR) * (XVAR) + (YVAR) * (YVAR)))
#define MAX(XVAR, YVAR) ((XVAR) > (YVAR) ? (XVAR) : (YVAR))
#define MIN(XVAR, YVAR) ((XVAR) < (YVAR) ? (XVAR) : (YVAR))

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

/**
	@enum layer
	@brief Entity layer enum
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

/*
Class structure type
*/
typedef union clist_u CList_t;
typedef union qtree_u QTree_t;
typedef union window_u Window_t;
typedef union entity_u Entity_t;

/*
Function definition
*/
static void *new(type_t type, ...);
static void delete(void *ptr);

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

/*
QTREE_CLASS
*/
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

/*
WINDOW_CLASS
*/
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

/*
ENTITY_CLASS
*/
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
	float lenght;
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

/**
	@relates clist_s
	@fn static void clist_t__push(CList_t *self, void *content)
	@brief Head push
	@param self Object pointer
	@param content Element pointer
	@return void
*/
static void clist_t__push(CList_t *self, void *content)
{
	clist_block_t *block = NULL;
	
	block = calloc(1, sizeof(clist_block_t));
	
	block->content = content;
	block->next = self->clist.head;
	
	self->clist.head = block;
}

/**
	@relates clist_s
	@fn static void *clist_t__pop(CList_t *self)
	@brief Head pop
	@param self Object pointer
	@return Head element pointer
*/
static void *clist_t__pop(CList_t *self)
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
	@fn static void *clist_t__iter(CList_t *self, clist_block_t **block)
	@brief Iterate chained list elements
	@param self Object pointer
	@param block Link pointer
	@return Element pointer
	
	@todo explain/example
*/
static void *clist_t__iter(CList_t *self, clist_block_t **block)
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
	@fn static void clist_t__remove(CList_t *self, void *content)
	@brief Remove element
	@param self Object pointer
	@param content Element pointer
	@return void
*/
static void clist_t__remove(CList_t *self, void *content)
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
	@fn static void clist_t__empty(CList_t *self)
	@brief Empty CList
	@param self Object pointer
	@return void
*/
static void clist_t__empty(CList_t *self)
{
	void *content = NULL;
	
	content = self->clist.pop(self);
	while (content)
		content = self->clist.pop(self);
}

/**
	@relates clist_s
	@fn static void clist_t__entityUpdateAndDraw(CList_t *self, layer_t updateLayer, layer_t drawLayer)
	@brief Update and draw enities selected by layer
	@param self Object pointer
	@param updateLayer Layers to update
	@param drawLayer Layers to draw
	@return void
*/
static void clist_t__entityUpdateAndDraw(CList_t *self, layer_t updateLayer, layer_t drawLayer)
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

/**
	@relates qtree_s
	@fn static void qtree_t__insert(QTree_t *self, Entity_t *content)
	@brief Insert element
	@param self Object pointer
	@param content Element pointer
	@return void
*/
static void qtree_t__insert(QTree_t *self, Entity_t *content)
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
	@fn static uint8_t qtree_t__remove(QTree_t *self, Entity_t *content)
	@brief Remove element
	@param self Object pointer
	@param content Element pointer
	@return Boolean for recurrent algorithm
*/
static uint8_t qtree_t__remove(QTree_t *self, Entity_t *content)
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
	@fn static CList_t *qtree_t__fetch(QTree_t *self, SDL_FRect rect)
	@brief Fetch elements in rect area
	@param self Object pointer
	@param rect Area
	@return Chained list of elements
*/
static CList_t *qtree_t__fetch(QTree_t *self, SDL_FRect rect)
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
	@fn static void qtree_t__update(QTree_t *self)
	@brief Update elements position in quadtree and clean unused quadtree
	@param self Object pointer
	@return void
*/
static void qtree_t__update(QTree_t *self)
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
	@fn static void qtree_t__draw(QTree_t *self, Window_t *window)
	@brief Draw quadtree area
	@param self Object pointer
	@param window Window pointer
	@return void
	
	@warning Use it only for debugging
*/
static void qtree_t__draw(QTree_t *self, Window_t *window)
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


/**
	@relates window_s
	@fn static void window_t__putOnCamera(Window_t *self, Entity_t *content)
	@brief Put content to draw on the window
	@param self Object pointer
	@param content Element pointer
	@return void
*/
static void window_t__putOnCamera(Window_t *self, Entity_t *content)
{
	SDL_FRect	  rect;
	SDL_FRect	  shadowrect;
	SDL_Texture	*shadow = NULL;

	rect = content->entity.getTextureRect(content);
	shadow = content->entity.getLighting(content);
	shadowrect = content->entity.getLightingRect(content);

	if (shadow)
	{
		SDL_SetRenderTarget(
			self->window.renderer,
			self->window.camera.shadow
		);
		
		SDL_RenderCopyF(
			self->window.renderer,
			shadow,
			NULL,
			&shadowrect
		);
	}
	
	SDL_SetRenderTarget(
		self->window.renderer,
		self->window.camera.texture
	);
	
	SDL_RenderCopyF(
		self->window.renderer,
		content->entity.getTexture(content),
		NULL,
		&rect
	);
	
#ifdef DEBUG_BOX
	SDL_SetRenderDrawColor(self->window.renderer, 0, 255, 0, 255);
	SDL_RenderDrawRectF(
		self->window.renderer,
		&rect
	);
#endif
}

/**
	@relates window_s
	@fn static uint8_t window_t__update(Window_t *self)
	@brief Update the window and draw all elements
	@param self Object pointer
	@return Boolean TRUE if quit signal is recieved
*/
static uint8_t window_t__update(Window_t *self)
{
	uint64_t time;
	
	time = SDL_GetTicks64();
	self->window.deltatime = time - self->window.time;
	self->window.time = time;
	
	if (!SDL_PollEvent(&(self->window.event)))
		self->window.event.type = SDL_FIRSTEVENT;
	
	SDL_SetRenderTarget(self->window.renderer, NULL);
	SDL_RenderCopyF(self->window.renderer, self->window.camera.texture, NULL, NULL);
	SDL_RenderCopyF(self->window.renderer, self->window.camera.shadow, NULL, NULL);
	SDL_RenderPresent(self->window.renderer);
	
	SDL_SetRenderTarget(self->window.renderer, self->window.camera.texture);
	SDL_SetRenderDrawColor(self->window.renderer, 0, 0, 0, 255);
	SDL_RenderClear(self->window.renderer);

	SDL_SetRenderTarget(self->window.renderer, self->window.camera.shadow);
	SDL_SetRenderDrawColor(
		self->window.renderer,
		self->window.camera.lighting.r,
		self->window.camera.lighting.g,
		self->window.camera.lighting.b,
		self->window.camera.lighting.a
	);
	SDL_RenderClear(self->window.renderer);

#ifdef FPS_ECO
	SDL_Delay(16);
#endif
	
	return !(self->window.event.type == SDL_QUIT);
}

/**
	@relates window_s
	@fn static SDL_Event window_t__getEvent(Window_t *self)
	@brief Get window event
	@param self Object pointer
	@return Window event
*/
static SDL_Event window_t__getEvent(Window_t *self)
{
	return self->window.event;
}

/**
	@relates window_s
	@fn static uint64_t window_t__getDeltatime(Window_t *self)
	@brief Get window deltatime
	@param self Object pointer
	@return Window deltatime
*/
static uint64_t window_t__getDeltatime(Window_t *self)
{
	return self->window.deltatime;
}

/**
	@relates window_s
	@fn static void window_t__setLighting(Window_t *self, SDL_Color color)
	@brief Set window lighting
	@param self Object pointer
	@param color Lighting propreties
	@return void
*/
static void window_t__setLighting(Window_t *self, SDL_Color color)
{
	self->window.camera.lighting = color;
}

/**
	@relates entity_s
	@fn static void entity_t__draw(Entity_t *self)
	@brief Draw element on the screen
	@param self Object pointer
	@return void
*/
static void entity_t__draw(Entity_t *self)
{
	self->entity.window->window.putOnCamera(self->entity.window, self);
}

/**
	@relates entity_s
	@fn static CList_t *entity_t__states(Entity_t *self)
	@brief Fetch Entity automata states
	@param self Object pointer
	@return CList_t of entity_state_t pointers
*/
static CList_t *entity_t__states(Entity_t *self)
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
	@fn static void entity_t__transition(Entity_t *self, uint8_t from, uint32_t type, int32_t sym, action_t action, uint8_t to)
	@brief Add a new transition in the Entity automata
	@param self Object pointer
	@param self Source state id
	@param self Symbol type
	@param self Symbol sym
	@param self Transition effect
	@param self Destination state id
	@return void
*/
static void entity_t__transition(Entity_t *self, uint8_t from, uint32_t type, int32_t sym, action_t action, uint8_t to)
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
	@fn static uint8_t entity_t__update(Entity_t *self)
	@brief Update Entity internal automata
	@param self Object pointer
	@return Current state id of the Entity automata
*/
static uint8_t entity_t__update(Entity_t *self)
{
	uint8_t				flag = 0;
	QTree_t				*qtree = NULL;
	CList_t				*list = NULL;
	Entity_t			   *elem = NULL;
	uint64_t			   deltatime = 0;
	action_t			   action = NO_ACT;
	Window_t			   *window = NULL;
	SDL_FRect			  area;
	SDL_FRect			  rect;
	SDL_FRect			  elemrect;
	SDL_Event			  event;
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

	rect.x += self->entity.delta.x * deltatime;
	rect.y += self->entity.delta.y * deltatime;
	area.x += self->entity.delta.x * deltatime;
	area.y += self->entity.delta.y * deltatime;
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
			self->entity.delta.x * deltatime,
			self->entity.delta.y * deltatime
		);
	
	return self->entity.state->id;
}

/**
	@relates entity_s
	@fn static QTree_t *entity_t__getQTree(Entity_t *self)
	@brief Get entity qtree
	@param self Object pointer
	@return Entity qtree
*/
static QTree_t *entity_t__getQTree(Entity_t *self)
{
	return self->entity.qtree;
}

/**
	@relates entity_s
	@fn static void entity_t__setQTree(Entity_t *self, QTree_t *qtree)
	@brief Set entity qtree
	@param self Object pointer
	@param qtree New qtree pointer
	@return void
*/
static void entity_t__setQTree(Entity_t *self, QTree_t *qtree)
{
	self->entity.qtree = qtree;
}

/**
	@relates entity_s
	@fn static SDL_FPoint entity_t__getPosition(Entity_t *self)
	@brief Get entity position
	@param self Object pointer
	@return Entity position
*/
static SDL_FPoint entity_t__getPosition(Entity_t *self)
{
	SDL_FPoint point;
	
	point.x = self->entity.position.x;
	point.y = self->entity.position.y;
	
	return point;
}

/**
	@relates entity_s
	@fn static void entity_t__setDeltaPosition(Entity_t *self, float dx, float dy)
	@brief Set entity delta position
	@param self Object pointer
	@param dx Delta x position
	@param dy Delta y position
	@return void
*/
static void entity_t__setDeltaPosition(Entity_t *self, float dx, float dy)
{
	self->entity.position.x += dx;
	self->entity.position.y += dy;
}

/**
	@relates entity_s
	@fn static layer_t entity_t__getLayer(Entity_t *self)
	@brief Get entity layer
	@param self Object pointer
	@return Entity layer
*/
static layer_t entity_t__getLayer(Entity_t *self)
{
	return self->entity.position.layer;
}

/**
	@relates entity_s
	@fn static SDL_FRect entity_t__getTextureRect(Entity_t *self)
	@brief Get entity texture rect
	@param self Object pointer
	@return Entity texture rect
*/
static SDL_FRect entity_t__getTextureRect(Entity_t *self)
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
	@fn static SDL_Texture *entity_t__getTexture(Entity_t *self)
	@brief Get entity texture
	@param self Object pointer
	@return Entity texture
*/
static SDL_Texture *entity_t__getTexture(Entity_t *self)
{
	return self->entity.graphics.texture;
}

/**
	@relates entity_s
	@fn static SDL_FRect entity_t__getHitbox(Entity_t *self)
	@brief Get entity hitbox rect
	@param self Object pointer
	@return Entity hitbox rect
*/
static SDL_FRect entity_t__getHitbox(Entity_t *self)
{
	SDL_FRect rect;

	rect = self->entity.health.rect;
	rect.x += self->entity.position.x;
	rect.y += self->entity.position.y;

	return rect;
}

/**
	@relates entity_s
	@fn static void entity_t__setLighting(Entity_t *self, float radius, SDL_Color color)
	@brief Set entity lighting radius/color by processing texture
	@param self Object pointer
	@param radius Lighting radius
	@param color Lighting color
	@return void

	@warning This function is slow so don't call it often
*/
static void entity_t__setLighting(Entity_t *self, float radius, SDL_Color color)
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
	@fn static SDL_Texture *entity_t__getLighting(Entity_t *self)
	@brief Get entity lighting texture
	@param self Object pointer
	@return Entity lighting texture
*/
static SDL_Texture *entity_t__getLighting(Entity_t *self)
{
	return self->entity.graphics.shadow;
}

/**
	@relates entity_s
	@fn static SDL_FRect entity_t__getLightingRect(Entity_t *self)
	@brief Get entity Lighting rect
	@param self Object pointer
	@return Entity lighting rect
*/
static SDL_FRect entity_t__getLightingRect(Entity_t *self)
{
	SDL_FRect rect;

	rect = self->entity.getTextureRect(self);
	rect.x -= rect.w * (self->entity.graphics.radius - 1) / 2;
	rect.y -= rect.h * (self->entity.graphics.radius - 1) / 2;
	rect.w *= self->entity.graphics.radius;
	rect.h *= self->entity.graphics.radius;

	return rect;
}

/*
CONSTRUCTOR SECTION
*/

static retno_t clist_t__ctor(CList_t *self)
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

static retno_t qtree_t__ctor(QTree_t *self)
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

static retno_t window_t__ctor(Window_t *self)
{
#ifdef DEBUG
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN);
#endif
	
	if (SDL_Init(SDL_INIT_VIDEO))
		return FAILURE;
	
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
		return FAILURE;
	
	if (
		SDL_CreateWindowAndRenderer(
			800, 600,
			SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED,
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
	self->window.camera.rect.w = 400.0;
	self->window.camera.rect.h = 250.0;
	
	self->window.camera.texture = SDL_CreateTexture(
		self->window.renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		self->window.camera.rect.w,
		self->window.camera.rect.h
	);
	SDL_SetTextureBlendMode(self->window.camera.texture, SDL_BLENDMODE_BLEND);
	
	self->window.camera.shadow = SDL_CreateTexture(
		self->window.renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		self->window.camera.rect.w,
		self->window.camera.rect.h
	);
	SDL_SetTextureBlendMode(self->window.camera.shadow, SDL_BLENDMODE_MOD);

	self->window.camera.lighting.r = 255;
	self->window.camera.lighting.g = 255;
	self->window.camera.lighting.b = 255;
	self->window.camera.lighting.a = 255;
	
	self->window.putOnCamera = &window_t__putOnCamera;
	self->window.update = &window_t__update;
	self->window.getEvent = &window_t__getEvent;
	self->window.setLighting = &window_t__setLighting;
	self->window.getDeltatime = &window_t__getDeltatime;
	
	return SUCCESS;
}

static retno_t entity_t__ctor(Entity_t *self)
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

/*
DESTRUCTOR SECTION
*/

static retno_t clist_t__dtor(CList_t *self)
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

static retno_t qtree_t__dtor(QTree_t *self)
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

static retno_t window_t__dtor(Window_t *self)
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

static retno_t entity_t__dtor(Entity_t *self)
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

/*
NEW ALLOCATOR SECTION
*/

static void *new(type_t type, ...)
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

/*
DELETE DEALLOCATOR SECTION
*/

static void delete(void *self)
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
