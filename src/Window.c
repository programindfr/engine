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
	@file Window.c
*/

#include <Base.h>
#include <Entity.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdint.h>
#include <Window.h>

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

retno_t window_t__ctor(Window_t *self)
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

retno_t window_t__dtor(Window_t *self)
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
