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
#include "veteran.h"
#include "battlefield.h"

Veteran::Veteran(const SpriteCollection *spr_coll,PLAYER player): Unit(player) {
	float factor=0.8+0.2*rand()/RAND_MAX;
	if (player==PLAYER_1) {
		x=0;
		dx=factor*DX;
	} else {
		x=SCREEN_W;
		dx=-factor*DX;
	}

	bit_veteran_dead=spr_coll->get_sprite("veteran_unit_cadaver");
	anim_veteran=spr_coll->get_anim_cycle_iterator("veteran_walk",0.14);
	anim_veteran_fight=spr_coll->get_anim_cycle_iterator("veteran_fight",0.18);

	w=W;
	h=H;
	y=BattleField::BaseY-h;
	state=WALK;
	
	this->name="veteran";
	this->life=Life;
	this->collide=true;
}

Veteran::~Veteran() {}

void Veteran::post_message(MessageQueue *mess_queue) {
	assert(!dead);
	switch (state) {
	case STOP:
		if ((player==PLAYER_1 && x>SCREEN_W-BattleField::CastleW-w) || (player==PLAYER_2 && x<BattleField::CastleW)) mess_queue->push(Message(Message::EVENT_DAMAGE_PLAYER,this,this,NO_DELAY,Message::PRIORITY_NORMAL,PlayerDamage));
		state=WALK;
		break;
	case WALK:
		if ((player==PLAYER_1 && x>SCREEN_W-BattleField::CastleW-w) || (player==PLAYER_2 && x<BattleField::CastleW)) mess_queue->push(Message(Message::EVENT_DAMAGE_PLAYER,this,this,NO_DELAY,Message::PRIORITY_NORMAL,PlayerDamage));
		else x=x+dx;
		break;
	case FIGHT:
	case CADAVER:
		break;
	}
}

void Veteran::handle_message(const Message &mess,MessageQueue *mess_queue) {
	if (!dead) {
	switch (state) {
	case STOP:
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
				mess_queue->push(Message(Message::EVENT_ATTACK,mess.sender,this,NO_DELAY,Message::PRIORITY_NORMAL,rand()%Damage));
				mess_queue->push(Message(Message::EVENT_ENDFIGHT,this,this,5,Message::PRIORITY_NORMAL));
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
		default:
			std::cerr<<"stop/walk state:";
			Unit::handle_message(mess,mess_queue);
		}			
		break;
	case FIGHT:
		switch (mess.event) {
		case Message::EVENT_COLLISION:
			break;
		case Message::EVENT_ENDFIGHT:
			state=STOP;
			break;
		case Message::EVENT_ATTACK:
			life-=mess.data;
			if (life<=0) {
				collide=false;
				state=CADAVER;
				mess_queue->push(Message(Message::EVENT_DIE,this,this,30,Message::PRIORITY_LOWEST));
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


void Veteran::draw() {
	switch (state) {
	case STOP:
	case WALK:
		if (player==PLAYER_1) anim_veteran.get_next_bitmap()->draw(x,y);
		else anim_veteran.get_next_bitmap()->draw_flip_h(x,y);
		break;
	case FIGHT:
		if (player==PLAYER_1) anim_veteran_fight.get_next_bitmap()->draw(x,y);
		else anim_veteran_fight.get_next_bitmap()->draw_flip_h(x,y);
		break;	
	case CADAVER:
		bit_veteran_dead->draw(x-4,BattleField::BaseY-15);
		break;
	}
	
}

