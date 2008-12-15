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
#include "gamescreen.h"

#include "board.h"
#include "utils.h"
#include "background.h"
#include "battlefield.h"
#include "lifebar.h"
#include "foreground.h"

#define FONT_COLOR { 0x77, 0xd1, 0x00, 0 }

GameScreen::GameScreen(const SpriteCollection *spr_coll,const CombinaisonCollection *cmb_coll,const std::string &ttf_path,TextureIds ids,Background *background) {
	font=TTF_OpenFont(ttf_path.c_str(),80);
	font_huge=TTF_OpenFont(ttf_path.c_str(),120);
	font_tiny=TTF_OpenFont(ttf_path.c_str(),40);
	if (!font || !font_huge || !font_tiny) {
		std::cerr<<"font "<<ttf_path<<" creation failed..."<<std::endl;
		return;
	}

	SDL_Color color=FONT_COLOR;
	SDL_Surface *text_p1_won_surf=TTF_RenderText_Solid(font_huge,"PL I WON!!!", color);
	SDL_Surface *text_p2_won_surf=TTF_RenderText_Solid(font_huge,"PL II WON!!!", color);
	SDL_Surface *text_key_help_surf=TTF_RenderText_Solid(font_tiny,"Space to continue ... Esc to quit", color);
	text_p1_won=new Sprite(text_p1_won_surf,ids[0]);
	text_p2_won=new Sprite(text_p2_won_surf,ids[1]);
	text_key_help=new Sprite(text_key_help_surf,ids[2]);
	skull=spr_coll->get_anim_cycle_iterator("skull",0.1);
	hand=spr_coll->get_anim_cycle_iterator("hand",0.1);
	SDL_FreeSurface(text_p1_won_surf);
	SDL_FreeSurface(text_p2_won_surf);
	SDL_FreeSurface(text_key_help_surf);

	score_id=ids[3];

	this->spr_coll=spr_coll;
	this->cmb_coll=cmb_coll;
	this->background=background;
}

GameScreen::~GameScreen() {
	delete text_p1_won;
	delete text_p2_won;
	delete text_key_help;
	
	TTF_CloseFont(font);
	TTF_CloseFont(font_huge);
	TTF_CloseFont(font_tiny);
}

void GameScreen::display_game(SDL_Surface *screen) {
	int p1_win=0;
	int p2_win=0;

	bool quit_game=false;
	SDL_Event event;
	SDL_Color color=FONT_COLOR;
	Uint32 ticks=SDL_GetTicks();

	while (!quit_game) {
		//game object init
		bool quit=false;
		PLAYER winner=PLAYER_NEUTRAL;
		LifeBar life_bar1(spr_coll,PLAYER_1);
		LifeBar life_bar2(spr_coll,PLAYER_2);
		Foreground foreground(spr_coll);
		BattleField battlefield(spr_coll,&life_bar1,&life_bar2,&foreground);
		Board board1(spr_coll,cmb_coll,&battlefield,PLAYER_1);
		Board board2(spr_coll,cmb_coll,&battlefield,PLAYER_2);
		
		//main loop
		while (!quit && !quit_game) {
			//draw
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			background->draw();
			life_bar1.draw();
			life_bar2.draw();
			battlefield.draw();
			board1.draw();
			board2.draw();
			foreground.draw();
			SDL_GL_SwapBuffers();	
			SDL_Flip(screen);
			
			//logic
			board1.logic();
			board2.logic();
			
			while (SDL_PollEvent(&event)) {
				switch (event.type) {
				case SDL_KEYDOWN:
					if (event.key.keysym.sym==SDLK_ESCAPE) quit_game=true;
					else if (event.key.keysym.sym==SDLK_t) life_bar2.damage(100); //DEBUG
					else if (event.key.keysym.sym==SDLK_y) life_bar1.damage(100);
					break;
				case SDL_QUIT:			
					quit_game=true;
					break;
				default:
					break;
				}
			}

			if (life_bar1.get_life()<=0) {
				winner=PLAYER_2;
				quit=true;
			} else if (life_bar2.get_life()<=0) {
				winner=PLAYER_1;
				quit=true;
			}

			while (ticks>(SDL_GetTicks()-1000/FPS)) SDL_Delay(3);
			ticks=SDL_GetTicks();
		}

		//final screen
		const Sprite *winning_message=NULL;
		switch (winner) {
		case PLAYER_1:
			p1_win++;
			winning_message=text_p1_won;
			break;
		case PLAYER_2:
			p2_win++;
			winning_message=text_p2_won;
			break;
		default:
			break;
		}
		
		//render score
		SDL_Surface *score_surf=TTF_RenderText_Solid(font,(number_as_roman(p1_win)+" : "+number_as_roman(p2_win)).c_str(),color);
		Sprite score_sprite(score_surf,score_id);
		SDL_FreeSurface(score_surf);

		quit=false;
		while (!quit && !quit_game) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//background
			background->draw();
			life_bar1.draw();
			life_bar2.draw();
			battlefield.draw();
			board1.draw();
			board2.draw();
			foreground.draw();
			
			//overlay
			fill_rect_opengl(0,0,SCREEN_W,SCREEN_H,0,0,0,0.7);
			winning_message->draw((SCREEN_W-winning_message->w)/2,50);
			text_key_help->draw((SCREEN_W-text_key_help->w)/2,SCREEN_H-50-text_key_help->h);
			score_sprite.draw((SCREEN_W-score_sprite.w)/2,(SCREEN_H-score_sprite.h)/2);

			const Sprite* current_skull=skull.get_next_bitmap();
			const Sprite* current_hand=hand.get_next_bitmap();
			if (winner==PLAYER_2) {
				current_skull->draw(SCREEN_W/2-score_sprite.w/2-80-current_skull->w/2,(SCREEN_H-current_skull->h)/2);
				current_hand->draw(SCREEN_W/2+score_sprite.w/2+80-current_skull->w/2,(SCREEN_H-current_skull->h)/2);
			} else {
				current_hand->draw(SCREEN_W/2-score_sprite.w/2-80-current_skull->w/2,(SCREEN_H-current_skull->h)/2);
				current_skull->draw(SCREEN_W/2+score_sprite.w/2+80-current_skull->w/2,(SCREEN_H-current_skull->h)/2);
			}

			SDL_GL_SwapBuffers();
			SDL_Flip(screen);

			while (SDL_PollEvent(&event)) {
				switch (event.type) {
				case SDL_KEYDOWN:
					if (event.key.keysym.sym==SDLK_ESCAPE) quit_game=true; //quit game
					if (event.key.keysym.sym==SDLK_SPACE) quit=true;
					break;
				case SDL_QUIT:			
					quit_game=true;
					break;
				default:
					break;
				}
			}

			SDL_Delay(10);
			while (ticks>(SDL_GetTicks()-1000/FPS)) SDL_Delay(3);
			ticks=SDL_GetTicks();
		}
	}
}
