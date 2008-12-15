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
#ifndef __SPRITE_H
#define __SPRITE_H

#include "plateformSDL.h"
#include "param.h"

class Sprite {
public:
	Sprite(SDL_Surface *surf, TextureId id);
	void draw(float x, float y, float alpha=1.0, float angle=0.0, float factor=1.0) const;
	void draw_flip_h(float x, float y, float alpha=1.0, float angle=0.0, float factor=1.0) const;
	float factor_w,factor_h;
	float w,h;
	unsigned int texture_id;
private:
};

#endif
