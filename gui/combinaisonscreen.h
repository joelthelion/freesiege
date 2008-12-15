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
#ifndef __COMBINAISONSCREEN_H
#define __COMBINAISONSCREEN_H

#include "spritecollection.h"
#include "combinaisoncollection.h"
#include "sprite.h"
#include <map>
#include <vector>

class CombinaisonScreen {
public:
	CombinaisonScreen(const SpriteCollection *spr_coll,const CombinaisonCollection *cmb_coll,const std::string &ttf_path,TextureIds ids);
	~CombinaisonScreen();
	void display_combinaisons(SDL_Surface *screen);
private:
	typedef std::vector<const Combinaison*> Combinaisons;
	struct CombinaisonSprite {
		const Sprite *name_normal_sprite;
		const Sprite *name_selected_sprite;
		const Sprite *game_sprite;
		Combinaisons combinaisons;
		Combinaisons::const_iterator current_combinaison;
	};
	typedef std::map<std::string*,CombinaisonSprite*> CombinaisonSprites;
	CombinaisonSprites combinaison_sprites;

	const Sprite *fire;
	const Sprite *wood;
	const Sprite *steel;
	const Sprite *magic;
	const Sprite *any;
};

#endif
