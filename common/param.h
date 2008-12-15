//	This file is part of freesiege program <http://freesiege.sourceforge.net>.
//	Copyright (C) 2007 Pierre Gueth <pierregueth@users.sourceforge.net>
//	                   Joel Schaerer <joelschaerer@users.sourceforge.net>
//
//	This program is free software: you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation, either version 3 of the License, or
//	(at your option) any later version.
//
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#ifndef __PARAM_H
#define __PARAM_H

#define SCREEN_W 800
#define SCREEN_H 600
#define SCREEN_DEPTH 32

#define FPS 40

#define FIELD_UPDATE_FRAME_SKIP 1 //skip UPDATE_FRAME_SKIP between update
#define BACK_UPDATE_FRAME_SKIP 2 

//TEXTURE parameters
typedef unsigned int TextureId;
typedef TextureId* TextureIds;
#define N_TEXTURE 512

//BOARD parameters
#define BOARD_BORDER 5
#define BOARD_H 5  //Board grid height
#define BOARD_W 6  //Board grid width
#define ELEM_H  50 //Height of element sprite
#define ELEM_W  50 //Width of element sprite
#define SPACING 10 //Spacing between element

typedef enum {PLAYER_1=1,PLAYER_2=2,PLAYER_NEUTRAL} PLAYER;
#define P2_KEY_SELECT SDLK_o	//key used to select the first element
#define P2_KEY_SWAP SDLK_o	//key used to select the second element and swap it with the first one
#define P2_KEY_VALIDATE SDLK_p	//key used to validate combinaisons and to spawn units on the battlefield
#define P2_KEY_LEFT SDLK_LEFT
#define P2_KEY_RIGHT SDLK_RIGHT
#define P2_KEY_DOWN SDLK_DOWN
#define P2_KEY_UP SDLK_UP

#ifdef WIN32
	#define P1_KEY_SELECT SDLK_q	//key used to select the first element
	#define P1_KEY_SWAP SDLK_q	//key used to select the second element and swap it with the first one
	#define P1_KEY_VALIDATE SDLK_w	//key used to validate combinaisons and to spawn units on the battlefield
#else
	#define P1_KEY_SELECT SDLK_a	//key used to select the first element
	#define P1_KEY_SWAP SDLK_a	//key used to select the second element and swap it with the first one
	#define P1_KEY_VALIDATE SDLK_z	//key used to validate combinaisons and to spawn units on the battlefield
#endif
#define P1_KEY_LEFT SDLK_s
#define P1_KEY_RIGHT SDLK_f
#define P1_KEY_DOWN SDLK_d
#define P1_KEY_UP SDLK_e

#define WOOD 'W'
#define STEEL 'S'
#define MAGIC 'M'
#define FIRE 'F'
#define ANY '.'
#define NONE 'x'
#define USED 'x'

//LIFEBAR parameters
#define LIFE_Y 320
#define LIFE_INNER_BORDER 3 
#define LIFE_W 360
#define LIFE_H 39
#define LIFE_BORDER 5
#define LIFE_FACTOR 60
#define LIFE_DECREASE 0.002

#endif
