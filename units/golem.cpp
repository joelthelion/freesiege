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
#include "golem.h"
#include "battlefield.h"

Golem::Golem(const SpriteCollection *spr_coll,PLAYER player): Unit(player) {
	w=W;
	h=H;

	float f=factor(0.8);
	if (player==PLAYER_1) {
		x=-w;
		dx=f*DX;
	} else {
		x=SCREEN_W;
		dx=-f*DX;
	}
	anim_golem_walk=spr_coll->get_anim_cycle_iterator("golem_walk",0.175);
	anim_golem_fight=spr_coll->get_anim_forward_backward_iterator("golem_fight",0.5);
	bit_golem_dead=spr_coll->get_sprite("golem_cadaver");

	y=BattleField::BaseY-h;
	state=WALK;
	
	this->name="golem";
	this->life=Life;
	this->collide=true;
}

Golem::~Golem() {}

void Golem::post_message(MessageQueue *mess_queue) {
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

void Golem::handle_message(const Message &mess,MessageQueue *mess_queue) {
	if (!dead) {
	switch (state) {
	case STOP:
	case WALK:
		switch (mess.event) {
		case Message::EVENT_DAMAGE_PLAYER:
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
				mess_queue->push(Message(Message::EVENT_SPAWN,this,this,NO_DELAY,Message::PRIORITY_NORMAL,EXPLOSION,x));
				mess_queue->push(Message(Message::EVENT_DIE,this,this,60,Message::PRIORITY_LOWEST));
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
				mess_queue->push(Message(Message::EVENT_SPAWN,this,this,NO_DELAY,Message::PRIORITY_NORMAL,EXPLOSION,x+w/2)); //x is the left boeder
				mess_queue->push(Message(Message::EVENT_DIE,this,this,60,Message::PRIORITY_LOWEST));
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
		case Message::EVENT_SPAWN:
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

void Golem::draw() {
	switch (state) {
	case STOP:
	case WALK:
		if (player==PLAYER_1) anim_golem_walk.get_next_bitmap()->draw(x,y);
		else anim_golem_walk.get_next_bitmap()->draw_flip_h(x,y);
		break;
	case FIGHT:
		if (player==PLAYER_1) anim_golem_fight.get_next_bitmap()->draw(x-10,y-5);
		else anim_golem_fight.get_next_bitmap()->draw_flip_h(x-10,y-5);
		break;	
	case CADAVER:
		bit_golem_dead->draw(x-11,BattleField::BaseY-26);
		break;
	}
	
}
