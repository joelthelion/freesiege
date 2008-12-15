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
#include "explosion.h"

#define EXPLOSION_DAMAGE 3
#define EXPLOSION_W 100
#define EXPLOSION_H 40
#define EXPLOSION_SPRITE_NUMBER 6 
#include "spritecollection.h"

Explosion::Explosion(const SpriteCollection *spr_coll,float x) : Unit(PLAYER_NEUTRAL) {
	bit_explosion_a=spr_coll->get_sprite("explosion_unit_a");
	bit_explosion_b=spr_coll->get_sprite("explosion_unit_b");
	
	this->w=EXPLOSION_W;
	this->h=EXPLOSION_H;
	
	collide=true;
	this->x=x-w/2;
	this->y=FIELD_BASE_Y-h;
	
	init=true;
	this->name="explosion";
	this->player=PLAYER_NEUTRAL;
}

Explosion::~Explosion() {}

void Explosion::post_message(MessageQueue *mess_queue) {
	if (init) {
		mess_queue->push(Message(Message::EVENT_DIE,this,NULL,4,Message::PRIORITY_NORMAL));
		init=false;
	}
}

void Explosion::handle_message(const Message &mess,MessageQueue *mess_queue) {
	switch (mess.event) {
	case Message::EVENT_DIE:
		dead=true;
		break;	
	case Message::EVENT_COLLISION:
		if (mess.sender->get_player()!=PLAYER_NEUTRAL) {
			mess_queue->push(Message(Message::EVENT_ATTACK,mess.sender,this,NO_DELAY,Message::PRIORITY_NORMAL,EXPLOSION_DAMAGE));
		}
		break;
	default:
		Unit::handle_message(mess,mess_queue);
	}
}

void Explosion::draw() {
	const Sprite *current_bitmap;
	for (int i=0;i<EXPLOSION_SPRITE_NUMBER;i++) {
		current_bitmap = (rand() % 2) ? bit_explosion_a : bit_explosion_b;
		float draw_x = rand() % (EXPLOSION_W-int(current_bitmap->w))+x;
		float draw_y = rand() % (EXPLOSION_H-int(current_bitmap->h))+y;
		current_bitmap->draw(draw_x,draw_y);
	}
}
