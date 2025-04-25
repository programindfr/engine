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
	@file layer.h
*/

#ifndef __LAYER_H__
#define __LAYER_H__

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

#endif/*__LAYER_H__*/
