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
#include "battlefield.h"

float Ball::X=14;
float Ball::Y=84;
float Ball::DX=7;
float Ball::DY=5;
float Ball::AY=0.12;
float Ball::Damage=80;

Ball::Ball(const SpriteCollection *spr_coll,PLAYER player) : Unit(player) {
	bit_ball=spr_coll->get_sprite("ball_unit");
	
	w=bit_ball->w;
	h=bit_ball->h;

	if (player==PLAYER_1) x=X;
	else x=SCREEN_W-X-w;

	float factor=0.5+0.5*rand()/RAND_MAX;
	y=BattleField::BaseY-Y;
	dy=-DY*factor;
	if (player==PLAYER_1) dx=DX*factor;
	else dx=-DX*factor;
	this->player=PLAYER_NEUTRAL;

	collide=true;
	name="ball";
}

Ball::~Ball() {}

void Ball::post_message(MessageQueue *mess_queue) {
	x+=dx;
	y+=dy;
	dy+=AY;
	if (y>=BattleField::BaseY-h) {
		y=BattleField::BaseY-h;
		mess_queue->push(Message(Message::EVENT_SPAWN,this,this,NO_DELAY,Message::PRIORITY_NORMAL,EXPLOSION,x));
		dead=true;
	}
}

void Ball::handle_message(const Message &mess,MessageQueue *mess_queue) {
	if (!dead) {
	switch (mess.event) {
	case Message::EVENT_COLLISION:
		if (mess.sender->get_player()!=PLAYER_NEUTRAL) {
			mess_queue->push(Message(Message::EVENT_ATTACK,mess.sender,this,NO_DELAY,Message::PRIORITY_NORMAL,Damage));
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
