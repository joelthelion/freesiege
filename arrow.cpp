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
#include "arrow.h"

#include "spritecollection.h"
#include <cassert>

#define ARROW_X 20
#define ARROW_Y 24

#define ARROW_DX 6
#define ARROW_DY 8
#define ARROW_AY 0.2
#define ARROW_LX 1

#define ARROW_DAMAGE 4

Arrow::Arrow(const SpriteCollection *spr_coll,PLAYER player,float pos_x) : Unit(player) {

	if (player==PLAYER_1)
	{
		x = pos_x+ARROW_X+ARROW_DX;
	}
	else
	{
		x = pos_x-ARROW_X-ARROW_DX; 
	}

	bit_arrow=spr_coll->get_sprite("arrow_unit");
	bit_arrow1=spr_coll->get_sprite("arrow_unit_1");
	bit_arrow2=spr_coll->get_sprite("arrow_unit_2");

	w=bit_arrow->w;
	h=bit_arrow->h;

	float f=factor(0.5);
	y=FIELD_BASE_Y-ARROW_Y;
	dy=-ARROW_DY*f;

	if (player==PLAYER_1) dx=ARROW_DX*f;
	else dx=-ARROW_DX*f;

	this->player=player;

	collide=true;
	this->name="arrow";
}

Arrow::~Arrow() {}

void Arrow::post_message(MessageQueue *mess_queue) {
	assert(!dead);
	x+=dx;
	y+=dy;
	dy+=ARROW_AY;

	if (y>=FIELD_BASE_Y-h) {
		dead=true;
	}
}

void Arrow::handle_message(const Message &mess,MessageQueue *mess_queue) {
	if (!dead) {
	switch (mess.event) {
	case Message::EVENT_COLLISION:
		if (mess.sender->get_player()!=PLAYER_NEUTRAL) {
			mess_queue->push(Message(Message::EVENT_ATTACK,mess.sender,this,NO_DELAY,Message::PRIORITY_NORMAL,ARROW_DAMAGE));
			mess_queue->push(Message(Message::EVENT_DIE,this,this,NO_DELAY,Message::PRIORITY_NORMAL));
		}
		break;
	case Message::EVENT_SHOOT:
	case Message::EVENT_ATTACK:
	case Message::EVENT_SPAWN:
		break;
	case Message::EVENT_DIE:
		dead=true;
		break;
	default:
		std::cerr<<"Arrow/state:";
		Unit::handle_message(mess,mess_queue);
	}
	}
}

void Arrow::draw() {
	if (player==PLAYER_2)
	{
		if(dy>ARROW_LX)
	 	bit_arrow1->draw(x,y);
		else if(dy>-ARROW_LX)
		bit_arrow->draw(x,y);
		else
		bit_arrow2->draw(x,y);
	}
	else
	{
		if(dy>ARROW_LX)
	 	bit_arrow1->draw_flip_h(x,y);
		else if(dy>-ARROW_LX)
		bit_arrow->draw_flip_h(x,y);
		else
		bit_arrow2->draw_flip_h(x,y);
	}
}
