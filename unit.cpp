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
#include "unit.h"

#include <iostream>
#include "soldier.h"
#include "druid.h"
#include "ball.h"
#include "knight.h"
#include "explosion.h"
#include "golem.h"
#include "plant.h"
#include "dragon.h"
#include "flame.h"
#include "veteran.h"
#include "archer.h"
#include "arrow.h"
#include "catapult.h"
#include "stone.h"

Unit *Unit::spawn(UNIT_ID name,const SpriteCollection *spr_coll,PLAYER player,float x) {
	switch (name) {
	case SOLDIER:
		return new Soldier(spr_coll, player);
	case KNIGHT:
		return new Knight(spr_coll,player);
	case DRUID:
		return new Druid(spr_coll, player);
	case GOLEM:
		return new Golem(spr_coll, player);
	case PLANT:
		return new Plant(spr_coll, player);
	case DRAGON:
		return new Dragon(spr_coll, player);
	case BALL:
		return new Ball(spr_coll, player);
	case EXPLOSION:
		return new Explosion(spr_coll, x);
	case FLAME:
		return new Flame(spr_coll, player, x);
	case ARCHER:
		return new Archer(spr_coll, player);
	case ARROW:
		return new Arrow(spr_coll, player, x);
	case VETERAN:
		return new Veteran(spr_coll, player);
	case CATAPULT:
		return new Catapult(spr_coll, player);
	case STONE:
		return new Stone(spr_coll, player, x);
	default:
		return NULL;
	}
}

Unit::Unit(PLAYER player) {
	this->player=player;
	this->dead=false;
	this->collide=true;
	this->name="abstract unit";
}

Unit::~Unit() {
}

void Unit::post_message(MessageQueue *mess_queue) {
	std::cerr<<"posting message on abstract unit!!!"<<std::endl;
}

void Unit::handle_message(const Message &mess,MessageQueue *mess_queue) {
	std::cerr<<"handle \""<<mess<<"\" not supported!!!"<<std::endl;
}

void Unit::draw() {
	std::cerr<<"draw on abstract class!!!"<<std::endl;
}

bool Unit::is_dead() const {
	return this->dead;
}

PLAYER Unit::get_player() const {
	return this->player;
}

bool Unit::can_collide() const {
	return this->collide;
}
