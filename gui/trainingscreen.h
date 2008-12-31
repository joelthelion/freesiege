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
#ifndef __TRAININGSCREEN_H
#define __TRAININGSCREEN_H

#include "background.h"
#include "combinaisoncollection.h"

class TrainingScreen {
public:
	TrainingScreen(const SpriteCollection *spr_coll,const CombinaisonCollection *cmb_coll,const std::string &ttf_path,TextureIds ids,Background *background);
	~TrainingScreen();
	void display_game(SDL_Surface *screen);
protected:
    int level;
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
