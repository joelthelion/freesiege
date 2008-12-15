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
#ifndef __VETERAN_H
#define __VETERAN_H

#include "unit.h"

class Veteran: public Unit {
public:
	Veteran(const SpriteCollection *spr_coll,PLAYER player);
	virtual ~Veteran();
	virtual void post_message(MessageQueue *mess_queue);
	virtual void handle_message(const Message &mess,MessageQueue *mess_queue);
	virtual void draw();

    static const int Life=60;
    static const int Damage=8;
    static const int PlayerDamage=16;
    static const float W=40;
    static const float H=52;
    static const float DX=1;
private:
	typedef enum {STOP,WALK,FIGHT,CADAVER} State;

	State state;
	int life;
	Anim::CycleIterator anim_veteran;
	Anim::CycleIterator anim_veteran_fight;
	const Sprite *bit_veteran_dead;
	float dx;
};

#endif
