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
#include "ball.h"

#define BALL_X 14
#define BALL_Y 84
#define BALL_DX 7
#define BALL_DY 5
#define BALL_AY 0.12
#define BALL_DAMAGE 80

Ball::Ball(const SpriteCollection *spr_coll,PLAYER player) : Unit(player) {
	bit_ball=spr_coll->get_sprite("ball_unit");
	
	w=bit_ball->w;
	h=bit_ball->h;

	if (player==PLAYER_1) x=BALL_X;
	else x=SCREEN_W-BALL_X-w;

	float factor=0.5+0.5*rand()/RAND_MAX;
	y=FIELD_BASE_Y-BALL_Y;
	dy=-BALL_DY*factor;
	if (player==PLAYER_1) dx=BALL_DX*factor;
	else dx=-BALL_DX*factor;
	this->player=PLAYER_NEUTRAL;

	collide=true;
	name="ball";
}

Ball::~Ball() {}

void Ball::post_message(MessageQueue *mess_queue) {
	x+=dx;
	y+=dy;
	dy+=BALL_AY;
	if (y>=FIELD_BASE_Y-h) {
		y=FIELD_BASE_Y-h;
		mess_queue->push(Message(Message::EVENT_SPAWN,this,this,NO_DELAY,Message::PRIORITY_NORMAL,EXPLOSION,x));
		dead=true;
	}
}

void Ball::handle_message(const Message &mess,MessageQueue *mess_queue) {
	if (!dead) {
	switch (mess.event) {
	case Message::EVENT_COLLISION:
		if (mess.sender->get_player()!=PLAYER_NEUTRAL) {
			mess_queue->push(Message(Message::EVENT_ATTACK,mess.sender,this,NO_DELAY,Message::PRIORITY_NORMAL,BALL_DAMAGE));
			dead=true;
		}
		break;
	case Message::EVENT_DIE:
		dead=true;
		break;
	default:
		Unit::handle_message(mess,mess_queue);
	}
	}
}

void Ball::draw() {
	bit_ball->draw(x,y);
}
