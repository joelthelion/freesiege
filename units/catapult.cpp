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
#include "catapult.h"

#include "lifebar.h"

#include <cassert>
#include <cmath>

#define CATAPULT_LIFE 400
#define CATAPULT_ATTACK 5
#define CATAPULT_PLAYER_DAMAGE 5
#define CATAPULT_W 147
#define CATAPULT_H 68
#define CATAPULT_H2 112
#define CATAPULT_DX 1

Catapult::Catapult(const SpriteCollection *spr_coll,PLAYER player): Unit(player) {
	float factor=0.8+0.2*rand()/RAND_MAX;

	float factor2=220.0+160.0*rand()/RAND_MAX;

	if (player==PLAYER_1) {
		x=0;
		px=factor2;
		dx=factor*CATAPULT_DX;
	} else {
		x=SCREEN_W;
		px=SCREEN_W-factor2;
		dx=-factor*CATAPULT_DX;
	}
	
	bit_catapult=spr_coll->get_sprite("catapult_unit");
	bit_catapult_dead=spr_coll->get_sprite("catapult_unit_destroy");
	anim_catapult=spr_coll->get_anim_cycle_iterator("catapult_moving",0.22);
	anim_catapult_fight=spr_coll->get_anim_cycle_iterator("catapult_fight",0.10);


	w=CATAPULT_W;
	h=CATAPULT_H;
	y=FIELD_BASE_Y-h;
	state=WALK;
	
	init=true;
	this->name="catapult";
	this->life=CATAPULT_LIFE;
	this->collide=true;
	shoot=false;
}

Catapult::~Catapult() {}

void Catapult::post_message(MessageQueue *mess_queue) {

	assert(!dead);
	switch (state) {
	case STOP:
		break;
	case WALK:
		if ((player==PLAYER_1 && x>px) || (player==PLAYER_2 && x<px))
		{
			if (init) {
			mess_queue->push(Message(Message::EVENT_SHOOT,this,this,10+rand()%10,Message::PRIORITY_NORMAL));
			init=false;
			collide=true;
			state=STOP;
			}
		}
		else x=x+dx;
		break;
	case FIGHT:
	case CADAVER:
		break;
	}
}


void Catapult::handle_message(const Message &mess,MessageQueue *mess_queue) {
	if (!dead) {
	switch (state) {
	case STOP:
		switch (mess.event) {
		case Message::EVENT_DAMAGE_PLAYER:
			//collide=false;
			//state=CADAVER;
			//mess_queue->push(Message(Message::EVENT_DIE,this,this,60,Message::PRIORITY_LOW));
			dead=true;
			break;
		case Message::EVENT_SHOOT:
			state=FIGHT;
			mess_queue->push(Message(Message::EVENT_SHOOT,this,this,30+rand()%30,Message::PRIORITY_NORMAL));
		break;
		case Message::EVENT_COLLISION:
			if (mess.sender->get_player()!=this->player && mess.sender->get_player()!=PLAYER_NEUTRAL) {
				collide=true;
				state=FIGHT;
				mess_queue->push(Message(Message::EVENT_SHOOT,this,this,60+rand()%60,Message::PRIORITY_NORMAL));
			}
			break;
		case Message::EVENT_ATTACK:
			life-=mess.data;

			if (life<=0) {
				collide=false;
				state=CADAVER;
				mess_queue->push(Message(Message::EVENT_DIE,this,this,30,Message::PRIORITY_LOWEST));
			}
			break;
		case Message::EVENT_ENDFIGHT:
			state=STOP;
			shoot=false;
			mess_queue->push(Message(Message::EVENT_SHOOT,this,this,60+rand()%60,Message::PRIORITY_NORMAL));
			break;
		case Message::EVENT_SPAWN:
			break;
		default:
			std::cerr<<"stop/walk state:";
			Unit::handle_message(mess,mess_queue);
		}			
		break;
	case WALK:
		switch (mess.event) {
		case Message::EVENT_DAMAGE_PLAYER:
			//collide=false;
			//state=CADAVER;
			//mess_queue->push(Message(Message::EVENT_DIE,this,this,60,Message::PRIORITY_LOW));
			dead=true;
			break;
		case Message::EVENT_COLLISION:
			if (mess.sender->get_player()!=this->player && mess.sender->get_player()!=PLAYER_NEUTRAL) {
				collide=true;
				state=FIGHT;
				mess_queue->push(Message(Message::EVENT_SHOOT,this,this,60+rand()%60,Message::PRIORITY_NORMAL));	
			}
			break;
		case Message::EVENT_ATTACK:
			life-=mess.data;

			if (life<=0) {
				collide=false;
				state=CADAVER;
				mess_queue->push(Message(Message::EVENT_DIE,this,this,30,Message::PRIORITY_LOWEST));
			}
			break;
		case Message::EVENT_ENDFIGHT:
			state=STOP;
			shoot=false;
			mess_queue->push(Message(Message::EVENT_SHOOT,this,this,60+rand()%60,Message::PRIORITY_NORMAL));
			break;
		case Message::EVENT_SPAWN:
			break;
		default:
			std::cerr<<"stop/walk state:";
			Unit::handle_message(mess,mess_queue);
		}			
		break;
	case FIGHT:
		switch (mess.event) {
		case Message::EVENT_COLLISION:
			break;
		
		case Message::EVENT_ATTACK:
			life-=mess.data;

			if (life<=0) {
				collide=false;
				state=CADAVER;
				mess_queue->push(Message(Message::EVENT_DIE,this,this,30,Message::PRIORITY_LOWEST));
			}
			break;
		case Message::EVENT_SHOOT:
			if(!shoot)
			{
				shoot=true;
				mess_queue->push(Message(Message::EVENT_SPAWN,this,NULL,NO_DELAY,Message::PRIORITY_NORMAL,STONE,x));
				mess_queue->push(Message(Message::EVENT_ENDFIGHT,this,this,3,Message::PRIORITY_NORMAL));
			}
			break;
		case Message::EVENT_ENDFIGHT:
			state=STOP;
			mess_queue->push(Message(Message::EVENT_SHOOT,this,this,60+rand()%60,Message::PRIORITY_NORMAL));
			shoot=false;
			break;
		case Message::EVENT_SPAWN:
			break;
		default:
			std::cerr<<"fight state:";
			Unit::handle_message(mess,mess_queue);
		}
		break;		
	case CADAVER:
		switch (mess.event) {
		case Message::EVENT_COLLISION:
		case Message::EVENT_ENDFIGHT:
		case Message::EVENT_ATTACK:
		case Message::EVENT_SHOOT:
			break;
		case Message::EVENT_DIE:
			dead=true;
			break;
		default:
			std::cerr<<"cadaver state:";
			Unit::handle_message(mess,mess_queue);
		}
	}
	}
}

void Catapult::draw() {
	switch (state) {
	case STOP:
		if (player==PLAYER_1) bit_catapult->draw(x,FIELD_BASE_Y-CATAPULT_H);
		else bit_catapult->draw_flip_h(x,FIELD_BASE_Y-CATAPULT_H);
		break;
	case WALK:
		if (player==PLAYER_1) anim_catapult.get_next_bitmap()->draw(x,FIELD_BASE_Y-CATAPULT_H);
		else anim_catapult.get_next_bitmap()->draw_flip_h(x,FIELD_BASE_Y-CATAPULT_H);
		break;
	case FIGHT:
		if (player==PLAYER_1) anim_catapult_fight.get_next_bitmap()->draw(x,FIELD_BASE_Y-CATAPULT_H2);
		else anim_catapult_fight.get_next_bitmap()->draw_flip_h(x,FIELD_BASE_Y-CATAPULT_H2);
		break;	
	case CADAVER:
		bit_catapult_dead->draw(x-8,FIELD_BASE_Y-30);
		break;
	}
}
