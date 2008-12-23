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
#ifndef __RAM_H
#define __RAM_H

#include "unit.h"

class Ram: public Unit {
public:
	Ram(const SpriteCollection *spr_coll,PLAYER player);
	virtual ~Ram();
	virtual void post_message(MessageQueue *mess_queue);
	virtual void handle_message(const Message &mess,MessageQueue *mess_queue);
	virtual void draw();

    static const int Life=30;
    static const int Damage=5;
    static const int PlayerDamage=16;
    static const float W=80;
    static const float H=30;
    static const float DX=1;
private:
	typedef enum {STOP,WALK,FIGHT,CADAVER} State;

	State state;
	int life;
	Anim::CycleIterator anim_ram;
	Anim::CycleIterator anim_ram_fight;
	const Sprite *bit_ram_dead;
	float dx;
};

#endif
