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
#ifndef __GAMESCREEN_H
#define __GAMESCREEN_H

#include "background.h"
#include "spritecollection.h"
#include "combinaisoncollection.h"
#include "param.h"

class GameScreen {
public:
	GameScreen(const SpriteCollection *spr_coll,const CombinaisonCollection *cmb_coll,const std::string &ttf_path,TextureIds ids,Background *background);
	~GameScreen();
	void display_game(SDL_Surface *screen);
private:
	const SpriteCollection *spr_coll;
	const CombinaisonCollection *cmb_coll;
	Background *background;

	TTF_Font *font;
	TTF_Font *font_huge;
	TTF_Font *font_tiny;

	const Sprite *text_p1_won;
	const Sprite *text_p2_won;
	const Sprite *text_key_help;
	Anim::CycleIterator skull;
	Anim::CycleIterator hand;
	TextureId score_id;
};

#endif
