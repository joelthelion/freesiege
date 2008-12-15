//	This file is part of freesiege program <http://freesiege.sourceforge.net>.
//	Copyright (C) 2007 Pierre Gueth <pierregueth@users.sourceforge.net>
//	                   Joel Schaerer <joelschaerer@users.sourceforge.net>
//	                   Julien Dardenne <jdardenne@users.sourceforge.net>
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
#ifndef __PLANT_H
#define __PLANT_H

#include "unit.h"

class Plant: public Unit {
public:
	Plant(const SpriteCollection *spr_coll,PLAYER player);
	virtual ~Plant();
	virtual void post_message(MessageQueue *mess_queue);
	virtual void handle_message(const Message &mess,MessageQueue *mess_queue);
	virtual void draw();
	static void reset_hasplant();

    static const int Life=100;
    static const int Damage=12;
    static const float W=69;
    static const float H=64;
    static const float X=90;
private:
	typedef enum {NORMAL,FIGHT,CADAVER} State;

	State state;
	int life;
	Anim::CycleIterator anim_plant_fight;
	const Sprite *bit_plant;
	const Sprite *bit_plant_dead;
	static bool player1_hasplant;
	static bool player2_hasplant;

};

#endif
