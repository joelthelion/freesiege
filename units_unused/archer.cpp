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
#include "archer.h"

#define ARCHER_LIFE 30
#define ARCHER_ATTACK 5
#define ARCHER_PLAYER_DAMAGE 5
#define ARCHER_W 20
#define ARCHER_H 28
#define ARCHER_DX 2

Archer::Archer(const SpriteCollection *spr_coll,PLAYER player): Unit(player) {
	float factor=0.8+0.2*rand()/RAND_MAX;

	float factor2=140.0+160.0*rand()/RAND_MAX;

	if (player==PLAYER_1) {
		x=0;
		px=factor2;
		dx=factor*ARCHER_DX;
	} else {
		x=SCREEN_W;
		px=SCREEN_W-factor2;
		dx=-factor*ARCHER_DX;
	}
	
	bit_archer=spr_coll->get_sprite("archer_unit");
	bit_archer_dead=spr_coll->get_sprite("archer_unit_cadaver");
	anim_archer=spr_coll->get_anim_cycle_iterator("archer_walk",0.16);
	anim_archer_fight=spr_coll->get_anim_cycle_iterator("archer_fight",0.13);


	w=ARCHER_W;
	h=ARCHER_H;
	y=FIELD_BASE_Y-h;
	state=WALK;
	
	init=true;
	this->name="archer";
	this->life=ARCHER_LIFE;
	this->collide=true;
	this->player=player;
}

Archer::~Archer() {}

void Archer::post_message(MessageQueue *mess_queue) {

	assert(!dead);
	switch (state) {
	case STOP:
		break;
	case WALK:
		if ((player==PLAYER_1 && x>px) || (player==PLAYER_2 && x<px))
		{
			state=FIGHT;
			if (init) {
			mess_queue->push(Message(Message::EVENT_SHOOT,this,this,10+rand()%10,Message::PRIORITY_NORMAL));
			init=false;
			}
		}
		else x=x+dx;
		break;
	case FIGHT:
	case CADAVER:
		break;
	}
}


void Archer::handle_message(const Message &mess,MessageQueue *mess_queue) {
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
				mess_queue->push(Message(Message::EVENT_ENDFIGHT,this,this,5,Message::PRIORITY_NORMAL));
			}
			break;
		case Message::EVENT_ATTACK:
			if (mess.sender->get_player()!=this->player)
			life-=mess.data;

			if (life<=0) {
				collide=false;
				state=CADAVER;
				mess_queue->push(Message(Message::EVENT_DIE,this,this,30,Message::PRIORITY_LOWEST));
			}
			break;
		case Message::EVENT_ENDFIGHT:
			state=STOP;
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
// 				mess_queue->push(Message(Message::EVENT_ATTACK,mess.sender,this,NO_DELAY,Message::PRIORITY_NORMAL,rand()%SOLDIER_ATTACK));
				mess_queue->push(Message(Message::EVENT_ENDFIGHT,this,this,5,Message::PRIORITY_NORMAL));
			}
			break;
		case Message::EVENT_ATTACK:
			if(mess.sender->get_player()!=this->player)
			life-=mess.data;

			if (life<=0) {
				collide=false;
				state=CADAVER;
				mess_queue->push(Message(Message::EVENT_DIE,this,this,30,Message::PRIORITY_LOWEST));
			}
			break;
		case Message::EVENT_ENDFIGHT:
			state=STOP;
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
			if(mess.sender->get_player()!=this->player)
			life-=mess.data;

			if (life<=0) {
				collide=false;
				state=CADAVER;
				mess_queue->push(Message(Message::EVENT_DIE,this,this,30,Message::PRIORITY_LOWEST));
			}
			break;
		case Message::EVENT_SHOOT:
			mess_queue->push(Message(Message::EVENT_SPAWN,this,NULL,NO_DELAY,Message::PRIORITY_NORMAL,ARROW,x));
			mess_queue->push(Message(Message::EVENT_ENDFIGHT,this,this,3,Message::PRIORITY_NORMAL));
			break;
		case Message::EVENT_ENDFIGHT:
			state=STOP;
			mess_queue->push(Message(Message::EVENT_SHOOT,this,this,60+rand()%60,Message::PRIORITY_NORMAL));
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

void Archer::draw() {
	switch (state) {
	case STOP:
		if (player==PLAYER_1) bit_archer->draw(x,FIELD_BASE_Y-ARCHER_H-5);
		else bit_archer->draw_flip_h(x,FIELD_BASE_Y-ARCHER_H-5);
		break;
	case WALK:
		if (player==PLAYER_1) anim_archer.get_next_bitmap()->draw(x,FIELD_BASE_Y-ARCHER_H);
		else anim_archer.get_next_bitmap()->draw_flip_h(x,FIELD_BASE_Y-ARCHER_H);
		break;
	case FIGHT:
		if (player==PLAYER_1) anim_archer_fight.get_next_bitmap()->draw(x,FIELD_BASE_Y-ARCHER_H);
		else anim_archer_fight.get_next_bitmap()->draw_flip_h(x,FIELD_BASE_Y-ARCHER_H);
		break;	
	case CADAVER:
		bit_archer_dead->draw(x-4,FIELD_BASE_Y-8);
		break;
	}
}
