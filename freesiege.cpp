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

#include "utils.h"
#include "param.h"
#include "trainingscreen.h"
#include "gamescreen.h"
#include "menuscreen.h"
#include "combinaisonscreen.h"
#include "musiccollection.h"

SDL_Surface *screen;

bool display_usage=false;
void usage() {
	if (display_usage) return;
	std::cout<<"freesiege [--windowed] [--no-sound] [--help]"<<std::endl;
	std::cout<<"  --windowed: launch the game using a 800x600 window"<<std::endl;
	std::cout<<"  --no-sound: turn off sound"<<std::endl;
	std::cout<<"  --help: display this help"<<std::endl;
	display_usage=true;
	exit(0);
}

int main(int argc, char* argv[]) {
	//parameters parsing
	bool sound=true;
	bool fullscreen=true;
	std::string param;
	for (int k=1; k<argc; k++) {
		param=std::string(argv[k]);
		if (param=="--no-sound") sound=false;
		else if (param=="--windowed") fullscreen=false;
		else if (param=="--help") usage();
		else {
			std::cerr<<"unknown parameter: "<<argv[k]<<std::endl;
			usage();
		}
	}

	//random init
	init_random_gen();

	//SDL init;
	if (SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_OPENGL)==-1) {
		std::cerr<<"sdl init failed..."<<SDL_GetError()<<std::endl;
		return 1;
	}

	if (fullscreen) screen=SDL_SetVideoMode(SCREEN_W,SCREEN_H,SCREEN_DEPTH,SDL_OPENGL|SDL_DOUBLEBUF|SDL_FULLSCREEN);
	else screen=SDL_SetVideoMode(SCREEN_W,SCREEN_H,SCREEN_DEPTH,SDL_OPENGL|SDL_DOUBLEBUF);
	if (!screen) {
		std::cerr<<"video init failed..."<<std::endl;
		return 1;
	}
	SDL_WM_SetCaption("FrEEsIegE",NULL);
	SDL_ShowCursor(SDL_DISABLE);
	TextureIds texture_ids=init_opengl(screen->w,screen->h,N_TEXTURE);


	if (TTF_Init()==-1) {
		std::cerr<<"ttf init failed..."<<TTF_GetError()<<std::endl;
		return 1;
	}

	if (sound) {
		if (Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,1024)==-1) {
			std::cerr<<"mixer init failed..."<<Mix_GetError()<<std::endl;
			sound=false;
		}
	}
	
	std::string base_dir=get_base_dir();
	//object init	
	CombinaisonCollection cmb_coll(base_dir+"combi.cfg");	
    std::cout<<cmb_coll<<std::endl;
    MusicCollection music_coll(base_dir+"musics.cfg",base_dir,sound);
    std::cout<<music_coll<<std::endl;
	SpriteCollection spr_coll(base_dir+"sprites.cfg",base_dir+"anims.cfg",base_dir,&texture_ids[0]);
    std::cout<<spr_coll<<std::endl;
	
	Background background(&spr_coll);
	TrainingScreen training_screen(&spr_coll,&cmb_coll,base_dir+"chlorinar.ttf",&texture_ids[200],&background);
	GameScreen game_screen(&spr_coll,&cmb_coll,base_dir+"chlorinar.ttf",&texture_ids[200],&background);
	CombinaisonScreen combi_screen(&spr_coll,&cmb_coll,base_dir+"chlorinar.ttf",&texture_ids[250]);
	MenuScreen menu_screen(&spr_coll,base_dir+"chlorinar.ttf",&texture_ids[300]);

	//main loop
    music_coll.play_music("intro");
	MenuScreen::SELECTION selection=MenuScreen::QUIT;
	while (!menu_screen.display_menu(screen,selection)) {
		if (selection==MenuScreen::TWO_PLAYERS) {
            music_coll.play_random_music();
			game_screen.display_game(screen);
            music_coll.play_music("intro");
		} else if (selection==MenuScreen::COMBINAISONS) {
            music_coll.play_music("combi");
			combi_screen.display_combinaisons(screen);
            music_coll.play_music("intro");
		} else if (selection==MenuScreen::TRAINING) {
            music_coll.play_random_music();
			training_screen.display_game(screen);
            music_coll.play_music("intro");
		} else std::cerr<<"unknown selection "<<selection<<std::endl;
	}


	TTF_Quit();
	SDL_Quit();
	return 0;
}
