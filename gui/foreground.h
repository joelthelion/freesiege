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
#ifndef __FOREGROUND_H
#define __FOREGROUND_H

#include "spritecollection.h"
#include <list>

class Foreground {
public:
	Foreground(const SpriteCollection *spr_coll);
	~Foreground();
	void draw();
	//void set_state(STATE state);
	bool p1_flower;
	bool p2_flower;
private:
	class Flower {
	public:
		Flower(const Sprite* sprite,PLAYER player);
		bool update();
		void reset();
		void draw() const;
	private:
		const Sprite *sprite;
		PLAYER player;
		float x,y;
		float dy;
		float angle;
		float dangle;
	};
	typedef std::list<Flower*> Flowers;
	Flowers p1_flowers;
	Flowers p2_flowers;
	
	const Sprite *flower_spr;
};

#endif
