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
#include "plant.h"

#define PLANT_LIFE 100
#define PLANT_ATTACK 12
#define PLANT_W 69
#define PLANT_H 64
#define PLANT_X 90

bool Plant::player1_hasplant=false;
bool Plant::player2_hasplant=false;

void Plant::reset_hasplant(){
	player1_hasplant=false;
	player2_hasplant=false;
}

Plant::Plant(const SpriteCollection *spr_coll,PLAYER player): Unit(player) {

	w=PLANT_W;
	h=PLANT_H;
	y=FIELD_BASE_Y-h;
	
	if (player==PLAYER_1) {
		x=PLANT_X;
		if (player1_hasplant) {
			dead=true; //Cannot have two plants at the same time
		}
		else player1_hasplant = true;
	} else {
		x=SCREEN_W-w-PLANT_X;
		if (player2_hasplant) {
			dead=true; //Cannot have two plants at the same time
		}
		else player2_hasplant = true;
	}

	anim_plant_fight=spr_coll->get_anim_cycle_iterator("plant_fight",0.22);
	bit_plant=spr_coll->get_sprite("plant_unit");
	bit_plant_dead=spr_coll->get_sprite("plant_unit_cadaver");

	state=NORMAL;
	this->name="plant";
	this->life=PLANT_LIFE;
	this->collide=true;
}

Plant::~Plant() {}

void Plant::post_message(MessageQueue *mess_queue) {
}

void Plant::handle_message(const Message &mess,MessageQueue *mess_queue) {
	if (!dead) {
	switch (state) {
	case NORMAL:
		switch (mess.event) {
		case Message::EVENT_COLLISION:
			if (mess.sender->get_player()!=this->player && mess.sender->get_player()!=PLAYER_NEUTRAL) {
				state=FIGHT;
				mess_queue->push(Message(Message::EVENT_ATTACK,mess.sender,this,NO_DELAY,Message::PRIORITY_NORMAL,rand()%PLANT_ATTACK));
				mess_queue->push(Message(Message::EVENT_ENDFIGHT,this,this,5,Message::PRIORITY_NORMAL));
			}
			break;
		case Message::EVENT_ATTACK:
			life-=mess.data;
			if (life<=0) {
				collide=false;
				state=CADAVER;
				mess_queue->push(Message(Message::EVENT_DIE,this,this,30,Message::PRIORITY_LOWEST));
				if (player==PLAYER_1) player1_hasplant=false;
				else player2_hasplant = false;
			}
			break;
		default:
			std::cerr<<"normal state:";
			Unit::handle_message(mess,mess_queue);
		}			
		break;
	case FIGHT:
		switch (mess.event) {
		case Message::EVENT_COLLISION:
			break;
		case Message::EVENT_ENDFIGHT:
			state=NORMAL;
			break;
		case Message::EVENT_ATTACK:
			life-=mess.data;
			if (life<=0) {
				collide=false;
				state=CADAVER;
				mess_queue->push(Message(Message::EVENT_DIE,this,this,30,Message::PRIORITY_LOWEST));
				if (player==PLAYER_1) player1_hasplant=false;
				else player2_hasplant = false;
			}
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

void Plant::draw() {
	switch (state) {
	case NORMAL:
		if (player==PLAYER_1) bit_plant->draw(x,y);
		else bit_plant->draw_flip_h(x,y);
		break;
	case FIGHT:
		if (player==PLAYER_1) anim_plant_fight.get_next_bitmap()->draw(x,y);
		else anim_plant_fight.get_next_bitmap()->draw_flip_h(x,y);
		break;	
	case CADAVER:
		if (player==PLAYER_1) bit_plant_dead->draw(x,y);
		else bit_plant_dead->draw_flip_h(x,y);
		break;
	}
	
}
