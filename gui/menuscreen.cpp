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
#include "menuscreen.h"

#include "param.h"
#include "options.h"

#define MENUSCREEN_BASE_Y 0
#define MENUSCREEN_BASE_X 0
#define MENUSCREEN_FREE_FRAME_COUNT 15
#define MENUSCREEN_SIEGE_FRAME_COUNT 30
//~ #define MENUSCREEN_SWORD_FRAME_COUNT 55
#define MENUSCREEN_DELAY 5

#define D_SHIFT 0.008;
#define CASTLE_SHIFT 150
#define CASTLE_BIAS 45
#define HILL_SHIFT 30
#define HILL_BIAS 50

MenuScreen::MenuScreen(const SpriteCollection *spr_coll,const std::string &ttf_path,TextureIds ids) {
	this->spr_coll=spr_coll;

	//main menu
	Menu::Titles titles;
	//titles.push_back("human versus cpu");
	titles.push_back("Human versus human");
	titles.push_back("Survival mode");
	titles.push_back("Patterns");
	titles.push_back("Options");
	titles.push_back("Quit");
	main_menu=new Menu(titles,ttf_path,&ids[10]);

	//option menu
	titles.clear();
    ai_level=NORMAL;
	titles.push_back("AI Level: Knight");
	titles.push_back("Keys");
	titles.push_back(Options::soundOn()?"Sound: on":"Sound: off");
	titles.push_back("Fullscreen");
	titles.push_back("Return");
	option_menu=new Menu(titles,ttf_path,&ids[0]);

	titles.clear();
	titles.push_back("select swap PI:a PII:o");
	titles.push_back("validate PI:z PII:p");
	titles.push_back("up PI:e PII:UP");
	titles.push_back("down PI:d PII:DOWN");
	titles.push_back("right PI:f PII:RIGHT");
	titles.push_back("left PI:s PII:LEFT");
	key_menu=new Menu(titles,ttf_path,&ids[20]);

	menus.push(main_menu);
}

MenuScreen::~MenuScreen() {
	delete option_menu;
	delete main_menu;
	delete key_menu;
}

bool MenuScreen::display_menu(SDL_Surface *screen,SELECTION &selection) {
	Uint32 ticks=SDL_GetTicks();
	Uint32 frame_count=0;

	//logo sprites
	const Sprite *logo_free=spr_coll->get_sprite("logo_free");
	const Sprite *logo_siege=spr_coll->get_sprite("logo_siege");
	//~ const Sprite *logo_sword=spr_coll->get_sprite("logo_sword");
	//background sprites
	const Sprite *back_hill=spr_coll->get_sprite("title_foreground");
	const Sprite *back_castle=spr_coll->get_sprite("title_castle");
	const Sprite *back_sky=spr_coll->get_sprite("title_sky");

	SDL_Event event;
	float shift=1.0;
	while (true) {
		Menu *current_menu=menus.top();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//fill_rect_opengl(0,0,SCREEN_W,SCREEN_H,1,1,0,1);
		back_sky->draw(0,0);

		back_castle->draw(0,CASTLE_BIAS+CASTLE_SHIFT*shift);
		back_hill->draw(0,HILL_BIAS+HILL_SHIFT*shift);
		shift-=D_SHIFT;
		if (shift<0) shift=0;

		//draw_fadein(logo_sword,(SCREEN_W-logo_sword->w)/2,MENUSCREEN_BASE_Y-logo_sword->h/2-4,frame_count,MENUSCREEN_SWORD_FRAME_COUNT,30,4);
		draw_fadein(logo_free,MENUSCREEN_BASE_X,MENUSCREEN_BASE_Y,frame_count,MENUSCREEN_FREE_FRAME_COUNT,MENUSCREEN_DELAY);
		draw_fadein(logo_siege,SCREEN_W-logo_siege->w-MENUSCREEN_BASE_X,MENUSCREEN_BASE_Y,frame_count,MENUSCREEN_SIEGE_FRAME_COUNT,MENUSCREEN_DELAY);
		current_menu->draw();
		SDL_GL_SwapBuffers();
		SDL_Flip(screen);

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					if (!menus.empty()) menus.pop();
					if (menus.empty()) {
						selection=QUIT;
						return true;
					}
					break;
				case SDLK_RETURN:
				case SDLK_SPACE:
					if (current_menu==main_menu) {
						switch (current_menu->get_selected()->n) {
						case 0://2 players versus
							selection=TWO_PLAYERS;
							return false;
							break;
						case 1://training mode
							selection=TRAINING;
							return false;
							break;
						case 2://combinaisons
							selection=COMBINAISONS;
							return false;
							break;
						case 3://options
							menus.push(option_menu);
							break;
						case 4://quit
							selection=QUIT;
							return true;
							break;
						default:
							break;
						}
					} else if (current_menu==option_menu) {
						switch (current_menu->get_selected()->n) {
                        case 0://ai level
                            switch (ai_level) {
                              case EASY:
                                ai_level=NORMAL;
								current_menu->get_selected()->title="AI Level: Knight";
                                break;
                              case NORMAL:
                                ai_level=HARD;
								current_menu->get_selected()->title="AI Level: Galaad the Righteous";
                                break;
                              case HARD:
                                ai_level=EXTREME;
								current_menu->get_selected()->title="AI Level: Morgoth the Cruel";
                                break;
                              case EXTREME:
                                ai_level=EASY;
								current_menu->get_selected()->title="AI Level: Farmer";
                                break; }
                            break;
						case 1://keys
							menus.push(key_menu);
							break;
						case 2://Sound
							if (!Options::soundOn())
							{
								Mix_ResumeMusic();
								Options::setSound(true);
								current_menu->get_selected()->title="Sound: on";
							}
							else
							{
								Mix_PauseMusic();
								Options::setSound(false);
								current_menu->get_selected()->title="Sound: off";
							}
							break;
						case 4://return
							menus.pop();
							break;
						default:
							break;
						}
					} else if (current_menu==key_menu) {
					}
					break;
				case SDLK_UP:
					current_menu->previous();
					break;
				case SDLK_DOWN:
					current_menu->next();
					break;
				default:
					break;
				}
				break;
			case SDL_QUIT:			
				return true;
				break;
			default:
				break;
			}
		}

		while (ticks>(SDL_GetTicks()-1000/FPS)) SDL_Delay(3);
		ticks=SDL_GetTicks();
		frame_count++;
	}

	return true;
}
