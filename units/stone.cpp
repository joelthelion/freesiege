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
#include "stone.h"

#include "spritecollection.h"
#include <cassert>

#define STONE_X 100
#define STONE_Y 24

#define STONE_PY 80
#define STONE_DX 5
#define STONE_DY 5
#define STONE_AY 0.1
#define STONE_LX 1
#define STONE_PLAYER_DAMAGE 22
#define STONE_DAMAGE 4

Stone::Stone(const SpriteCollection *spr_coll,PLAYER player,float pos_x) : Unit(player) {

	if (player==PLAYER_1)
	{
		x = pos_x+STONE_DX;
	}
	else
	{
		x = pos_x+STONE_X+STONE_DX; 
	}

	bit_stone=spr_coll->get_sprite("stone_unit");

	w=bit_stone->w;
	h=bit_stone->h;

	float f=factor(0.5);
	y=FIELD_BASE_Y-STONE_Y-STONE_PY;
	dy=-STONE_DY*f;

	if (player==PLAYER_1) dx=STONE_DX*f;
	else dx=-STONE_DX*f;

	this->player=player;

	collide=true;
	this->name="stone";
}

Stone::~Stone() {}

void Stone::post_message(MessageQueue *mess_queue) {
	assert(!dead);
	x+=dx;
	y+=dy;
	dy+=STONE_AY;

	if (y>=FIELD_BASE_Y-h) {
		mess_queue->push(Message(Message::EVENT_SPAWN,this,this,NO_DELAY,Message::PRIORITY_NORMAL,EXPLOSION,x));
		dead=true;
	}

	if ((player==PLAYER_1 && x>SCREEN_W-FIELD_CASTLE_W-w) || (player==PLAYER_2 && x<FIELD_CASTLE_W)) mess_queue->push(Message(Message::EVENT_DAMAGE_PLAYER,this,this,NO_DELAY,Message::PRIORITY_NORMAL,STONE_DAMAGE));
}

void Stone::handle_message(const Message &mess,MessageQueue *mess_queue) {
	if (!dead) {
	switch (mess.event) {
	case Message::EVENT_COLLISION:
		if (mess.sender->get_player()!=PLAYER_NEUTRAL) {
			mess_queue->push(Message(Message::EVENT_ATTACK,mess.sender,this,NO_DELAY,Message::PRIORITY_NORMAL,STONE_DAMAGE));
			mess_queue->push(Message(Message::EVENT_DIE,this,this,NO_DELAY,Message::PRIORITY_NORMAL));
		}
		break;
	case Message::EVENT_ATTACK:
	case Message::EVENT_SPAWN:
		break;
	case Message::EVENT_DIE:
		dead=true;
		break;
	case Message::EVENT_DAMAGE_PLAYER:
			//collide=false;
			//state=CADAVER;
			//mess_queue->push(Message(Message::EVENT_DIE,this,this,60,Message::PRIORITY_LOW));
			dead=true;
			break;
	default:
		std::cerr<<"Stone/state:";
		Unit::handle_message(mess,mess_queue);
	}
	}
}

void Stone::draw() {
	if (player==PLAYER_2)
	{
		bit_stone->draw(x,y);
	}
	else
	{
		bit_stone->draw_flip_h(x,y);
	}
}
