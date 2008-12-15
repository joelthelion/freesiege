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
#include <iostream>
#include <string>

#include "background.h"
#include "battlefield.h"
#include "spritecollection.h"
#include "lifebar.h"
#include "param.h"
#include "foreground.h"

SDL_mutex *counter_reset_mutex;
int valid_fps;
int current_fps;

Uint32 counter_reset_callback(Uint32 interval,void *param) {
	SDL_LockMutex(counter_reset_mutex);
		valid_fps=current_fps;
		current_fps=0;
		std::cout<<"FPS "<<valid_fps<<std::endl;
	SDL_UnlockMutex(counter_reset_mutex);

	return interval;
}

SDL_Surface *screen;
int main(int argc, char* argv[]) {
	init_random_gen();
	//SDL init;
	std::cout<<"init sdl"<<std::endl;
	if (SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO|SDL_OPENGL|SDL_INIT_TIMER)==-1) {
		std::cerr<<"sdl init failed..."<<SDL_GetError()<<std::endl;
		return 1;
	}

	std::cout<<"init video"<<std::endl;
	screen=SDL_SetVideoMode(SCREEN_W,SCREEN_H,SCREEN_DEPTH,SDL_OPENGL|SDL_DOUBLEBUF);
	if (!screen) {
		std::cerr<<"video init failed..."<<std::endl;
		return 1;
	}
	SDL_WM_SetCaption("FrEEsIegE FiGHt",NULL);
	TextureIds texture_ids=init_opengl(screen->w,screen->h,N_TEXTURE);

	//object init
	std::string base_dir=get_base_dir();
	SpriteCollection spr_coll(base_dir+"sprites.cfg",base_dir+"anims.cfg",base_dir,texture_ids);
	std::cout<<spr_coll<<std::endl;
	
	Background background(&spr_coll);
	Foreground foreground(&spr_coll);
	LifeBar life_bar1(&spr_coll,PLAYER_1);
	LifeBar life_bar2(&spr_coll,PLAYER_2);
	BattleField battlefield(&spr_coll,&life_bar1,&life_bar2,&foreground);

	//mutex timer init
	counter_reset_mutex=SDL_CreateMutex();
	SDL_TimerID counter_reset_id=SDL_AddTimer(1000,counter_reset_callback,NULL);

	bool quit=false;
	SDL_Event event;
	Uint32 ticks=SDL_GetTicks();
	while (!quit) {
		//draw
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		background.draw();
		life_bar1.draw();
		life_bar2.draw();
		battlefield.draw();
		foreground.draw();
		SDL_GL_SwapBuffers();	
		SDL_Flip(screen);
		//logic
		
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				if (event.key.keysym.sym!=SDLK_ESCAPE) {
				switch (event.key.keysym.sym) {
				//unit spawn keys
				case SDLK_a:
					battlefield.spawn(SOLDIER,PLAYER_1);
					break;
				case SDLK_q:
					battlefield.spawn(SOLDIER,PLAYER_2);
					break;
				case SDLK_z:
					battlefield.spawn(DRUID,PLAYER_1);
					break;
				case SDLK_s:
					battlefield.spawn(DRUID,PLAYER_2);
					break;
				case SDLK_e:
					battlefield.spawn(KNIGHT,PLAYER_1);
					break;
				case SDLK_d:
					battlefield.spawn(KNIGHT,PLAYER_2);
					break;
				case SDLK_r:
					battlefield.spawn(GOLEM,PLAYER_1);
					break;
				case SDLK_f:
					battlefield.spawn(GOLEM,PLAYER_2);
					break;
				case SDLK_t:
					battlefield.spawn(PLANT,PLAYER_1);
					break;
				case SDLK_g:
					battlefield.spawn(PLANT,PLAYER_2);
					break;
				case SDLK_y:
					battlefield.spawn(DRAGON,PLAYER_1);
					break;
				case SDLK_h:
					battlefield.spawn(DRAGON,PLAYER_2);
					break;
				case SDLK_u:
					battlefield.spawn(FLOWER,PLAYER_2);
					break;
				case SDLK_j:
					battlefield.spawn(FLOWER,PLAYER_1);
					break;
				case SDLK_i:
					battlefield.spawn(ARCHER,PLAYER_2);
					break;
				case SDLK_k:
					battlefield.spawn(ARCHER,PLAYER_1);
					break;
				case SDLK_o:
					battlefield.spawn(VETERAN,PLAYER_2);
					break;
				case SDLK_l:
					battlefield.spawn(VETERAN,PLAYER_1);
					break;
				case SDLK_p:
					battlefield.spawn(CATAPULT,PLAYER_2);
					break;
				case SDLK_m:
					battlefield.spawn(CATAPULT,PLAYER_1);
					break;
				default:
					break;
				}
				break;
				}
			case SDL_QUIT:			
				quit=true;
				break;
			default:
				break;
			}
		}

		while (ticks>(SDL_GetTicks()-1000/FPS)) SDL_Delay(3);
		ticks=SDL_GetTicks();
		
		current_fps++;
	}	

	SDL_RemoveTimer(counter_reset_id);
	SDL_DestroyMutex(counter_reset_mutex);

	SDL_Quit();
	return 0;
}
