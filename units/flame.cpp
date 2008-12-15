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
#include "flame.h"
#include "battlefield.h"

float Flame::Y=170;
float Flame::DX=7;
float Flame::DY=5;
int Flame::Damage=15;

Flame::Flame(const SpriteCollection *spr_coll,PLAYER player,float pos_x) : Unit(player) {
	bit_flame=spr_coll->get_sprite("flame_unit");
	w=bit_flame->w;
	h=bit_flame->h;

	x=pos_x-w/2; 
	y=BattleField::BaseY-Y;

	float f=factor(0.5);
	if (player==PLAYER_1) dx=DX*f;
	else dx=-DX*f;
	dy=-DY*f;

	this->player=PLAYER_NEUTRAL;

	collide=true;
	this->name="flame";
}

Flame::~Flame() {}

void Flame::post_message(MessageQueue *mess_queue) {
	x+=dx;
	y-=dy;

	if (y>=BattleField::BaseY-h) {
		y=BattleField::BaseY-h;
		mess_queue->push(Message(Message::EVENT_SPAWN,this,this,NO_DELAY,Message::PRIORITY_NORMAL,EXPLOSION,x));
		dead=true;
	}
}

void Flame::handle_message(const Message &mess,MessageQueue *mess_queue) {
	if (!dead) {
		switch (mess.event) {
		case Message::EVENT_COLLISION:
			if (mess.sender->get_player()!=PLAYER_NEUTRAL && mess.sender->name!="dragon") {
				mess_queue->push(Message(Message::EVENT_ATTACK,mess.sender,this,NO_DELAY,Message::PRIORITY_NORMAL,Damage));
				mess_queue->push(Message(Message::EVENT_SPAWN,this,this,NO_DELAY,Message::PRIORITY_NORMAL,EXPLOSION,x));
			}
			break;
		case Message::EVENT_SPAWN:
		case Message::EVENT_DIE:
			dead=true;
			break;
		default:
			Unit::handle_message(mess,mess_queue);
		}
	}
}

void Flame::draw() {
	if (dx<0) bit_flame->draw(x,y);
	else bit_flame->draw_flip_h(x,y);
}
