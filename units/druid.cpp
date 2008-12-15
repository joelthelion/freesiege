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
#include "druid.h"
#include "battlefield.h"
#include "param.h"

Druid::Druid(const SpriteCollection *spr_coll,PLAYER player) : Unit(player) {
	bit_druid=spr_coll->get_sprite("druid_unit");
	if (player==PLAYER_1) {
		x=X;
	} else {
		x=SCREEN_W-X-bit_druid->w;
	}
	y=BattleField::BaseY-Y;
	w=bit_druid->w;
	h=bit_druid->h;

	name="druid";
	collide=false;
	state=INIT;
}

Druid::~Druid() {}

void Druid::post_message(MessageQueue *mess_queue) {
	switch (state) {
	case INIT:
		mess_queue->push(Message(Message::EVENT_SHOOT,this,NULL,30+rand()%30));
		state=WAIT;
		break;
	case WAIT:
		break;
	case SHOOTED:
		break;
	}
}

void Druid::handle_message(const Message &mess,MessageQueue *mess_queue) {
	switch (state) {
	case INIT:
		std::cerr<<"init state:";
		Unit::handle_message(mess,mess_queue);
		break;
	case WAIT:
		switch (mess.event) {
		case Message::EVENT_SHOOT:
			state=SHOOTED;
			mess_queue->push(Message(Message::EVENT_SPAWN,this,this,NO_DELAY,Message::PRIORITY_HIGH,BALL)); //Create THREE balls
			mess_queue->push(Message(Message::EVENT_SPAWN,this,this,NO_DELAY,Message::PRIORITY_HIGH,BALL));
			mess_queue->push(Message(Message::EVENT_SPAWN,this,this,NO_DELAY,Message::PRIORITY_HIGH,BALL));
			mess_queue->push(Message(Message::EVENT_DIE,this,this,30));
			break;
		default:
			std::cerr<<"wait state:";
			Unit::handle_message(mess,mess_queue);
		}
		break;
	case SHOOTED:
		switch (mess.event) {
		case Message::EVENT_SPAWN:
			break;
		case Message::EVENT_DIE:
			dead=true;
			break;
		default:
			std::cerr<<"shooted state:";
			Unit::handle_message(mess,mess_queue);
		}
	}
}

void Druid::draw() {
	if (player==PLAYER_1) bit_druid->draw(x,y);
	else bit_druid->draw_flip_h(x,y);
}
