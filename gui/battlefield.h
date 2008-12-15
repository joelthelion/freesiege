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
#ifndef __BATTLEFIELD_H
#define __BATTLEFIELD_H

#include <list>
#include "param.h"
#include "message.h"
#include "sprite.h"
#include "spritecollection.h"
#include "lifebar.h"
#include "foreground.h"

class BattleField {
public:
	BattleField(const SpriteCollection *spr_coll,LifeBar *lifebar1,LifeBar *lifebar2,Foreground * foreground);
	~BattleField();
	
	void draw();
	void spawn(UNIT_ID name,PLAYER player,float x=0);
private:
	typedef std::list<Unit*> Units;
	Units units;

	LifeBar *lifebar1;
	LifeBar *lifebar2;

	const SpriteCollection *spr_coll;
	MessageQueue *current_mess_queue;

	int frame_skip_count;
	Sprite *bit_castle;

	Foreground * foreground;
};

#endif
