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
#ifndef __MENU_H
#define __MENU_H

#include "sprite.h"

class Menu {
public:
	typedef std::vector<std::string> Titles;
	struct TitleSprite {
		int n;
		float y;
		std::string title;
		Sprite *spr_normal;
		Sprite *spr_selected;
	};

	Menu(const Titles &titles,const std::string &ttf_path,TextureIds ids);
	~Menu();
	void draw();
	void next();
	void previous();
	TitleSprite* get_selected() const;
private:
	typedef std::vector<TitleSprite*> TitleSprites;
	
    void update_text();
    std::string ttf_path;
	TitleSprites title_sprites;
	TitleSprites::const_iterator selected;
    TextureIds ids;
};

#endif
