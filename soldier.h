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
#ifndef __SOLDIER_H
#define __SOLDIER_H

#include "unit.h"

class Soldier: public Unit {
public:
	Soldier(const SpriteCollection *spr_coll,PLAYER player);
	virtual ~Soldier();
	virtual void post_message(MessageQueue *mess_queue);
	virtual void handle_message(const Message &mess,MessageQueue *mess_queue);
	virtual void draw();
private:
	typedef enum {STOP,WALK,FIGHT,CADAVER} State;

	State state;
	int life;
	Anim::CycleIterator anim_soldier;
	Anim::CycleIterator anim_soldier_fight;
	const Sprite *bit_soldier_dead;
	float dx;
};

#endif
