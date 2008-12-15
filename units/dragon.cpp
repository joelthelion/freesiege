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
#include "dragon.h"

#define DRAGON_LIFE 200
#define DRAGON_PLAYER_DAMAGE 6
#define DRAGON_Y 180 
#define DRAGON_W 174
#define DRAGON_H 58
#define DRAGON_DX 1

Dragon::Dragon(const SpriteCollection *spr_coll,PLAYER player): Unit(player) {
	w=DRAGON_W;
	h=DRAGON_H;

	float f=factor(0.8);
	if (player==PLAYER_1) {
		x=-w;
		dx=f*DRAGON_DX;
	} else {
		x=SCREEN_W;
		dx=-f*DRAGON_DX;
	}
	y=FIELD_BASE_Y-DRAGON_Y;

	anim_dragon_fly=spr_coll->get_anim_cycle_iterator("dragon_fly",0.175);
	bit_dragon_dead=spr_coll->get_sprite("dragon_cadaver");

	state=FLY;
	
	this->name="dragon";
	this->life=DRAGON_LIFE;
	this->collide=true;
	this->init=true;
	this->player = player;
}

Dragon::~Dragon() {}

void Dragon::post_message(MessageQueue *mess_queue) {
	switch (state) {
	case FLY:
		if ((player==PLAYER_1 && x>SCREEN_W-FIELD_CASTLE_W) || (player==PLAYER_2 && x<FIELD_CASTLE_W-w)) mess_queue->push(Message(Message::EVENT_DAMAGE_PLAYER,this,this,NO_DELAY,Message::PRIORITY_NORMAL,DRAGON_PLAYER_DAMAGE));
		else x=x+dx;
		
		if (init) {
			mess_queue->push(Message(Message::EVENT_SHOOT,this,this,60+rand()%60,Message::PRIORITY_NORMAL));
			init=false;
		}

		break;
	case CADAVER:
		if (y<FIELD_BASE_Y-DRAGON_H) { y+=5; x+=dx; }
		break;
	}
}

void Dragon::handle_message(const Message &mess,MessageQueue *mess_queue) {
	if (!dead) {
	switch (state) {
	case FLY:
		switch (mess.event) {
		case Message::EVENT_COLLISION:
		case Message::EVENT_SPAWN:
			break;
		case Message::EVENT_SHOOT:
			if (player==PLAYER_1) mess_queue->push(Message(Message::EVENT_SPAWN,this,NULL,NO_DELAY,Message::PRIORITY_NORMAL,FLAME,x+w));
			else mess_queue->push(Message(Message::EVENT_SPAWN,this,NULL,NO_DELAY,Message::PRIORITY_NORMAL,FLAME,x));
			mess_queue->push(Message(Message::EVENT_SHOOT,this,this,60+rand()%60,Message::PRIORITY_NORMAL));
			break;
		case Message::EVENT_DAMAGE_PLAYER:
			dead=true;
			break;
		case Message::EVENT_ATTACK:
			life-=mess.data;
			if (life<=0) {
				collide=false;
				state=CADAVER;
				mess_queue->push(Message(Message::EVENT_DIE,this,this,20,Message::PRIORITY_LOWEST));
			}
			break;
		default:
			std::cerr<<"fly state:";
			Unit::handle_message(mess,mess_queue);
		}			
		break;
	case CADAVER:
		switch (mess.event) {
		case Message::EVENT_SHOOT:
		case Message::EVENT_COLLISION:
		case Message::EVENT_ENDFIGHT:
		case Message::EVENT_ATTACK:
			break;
		case Message::EVENT_DIE:
			mess_queue->push(Message(Message::EVENT_SPAWN,this,this,NO_DELAY,Message::PRIORITY_NORMAL,EXPLOSION,x+w/2));
			break;
		case Message::EVENT_SPAWN:
			dead=true;
			break;
		default:
			std::cerr<<"cadaver state:";
			Unit::handle_message(mess,mess_queue);
		}
	}
	}
}

void Dragon::draw() {
	switch (state) {
	case FLY:
		if (player==PLAYER_1) anim_dragon_fly.get_next_bitmap()->draw(x,y-56);
		else anim_dragon_fly.get_next_bitmap()->draw_flip_h(x,y-56);
		break;
	case CADAVER:
		if (player==PLAYER_1) bit_dragon_dead->draw(x,y);
		else bit_dragon_dead->draw_flip_h(x,y);
		break;
	}
	
}
