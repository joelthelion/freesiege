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
#ifndef __UNIT_H
#define __UNIT_H

#include "param.h"
#include "message.h"
#include "utils.h"
#include "sprite.h"
#include "spritecollection.h"

class Unit {
public:
	static Unit *spawn(UNIT_ID name,const SpriteCollection *spr_coll,PLAYER player,float x=0);
	
	Unit(PLAYER player);
	virtual ~Unit();
	virtual void post_message(MessageQueue *mess_queue);
	virtual void handle_message(const Message &mess,MessageQueue *mess_queue);
	virtual void draw();
	bool is_dead() const;
	PLAYER get_player() const;
	bool can_collide() const;
	
	std::string name;
	float x,y;
	float w,h;
protected:
	PLAYER player;
	bool dead;
	bool collide;
};

#endif
