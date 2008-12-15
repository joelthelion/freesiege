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
#include "foreground.h"

#include "utils.h"
#include "param.h"

#define FLOWER_SPEED 7
#define FLOWER_ROT_SPEED 10 
#define FLOWER_FADEOUT_END_Y 400
#define FLOWER_FADEOUT_H 75
#define FLOWER_N 100

//flower nested class
Foreground::Flower::Flower(const Sprite* sprite,PLAYER player) {
	this->sprite=sprite;
	this->player=player;
	reset();
}

bool Foreground::Flower::update() {
	y+=dy;
	angle+=dangle;
	return y<FLOWER_FADEOUT_END_Y;
}

void Foreground::Flower::reset() {
	dangle=FLOWER_ROT_SPEED*(0.5-factor(0));
	angle=360*factor(0);
	dy=FLOWER_SPEED*factor(0.5);
	y=-sprite->h-FLOWER_FADEOUT_END_Y*factor(0);
	if (player==PLAYER_1) x=(SCREEN_W/2-sprite->w)*factor(0);
	else x=SCREEN_W/2+(SCREEN_W/2-sprite->w)*factor(0);
}

void Foreground::Flower::draw() const {
	draw_fadeout(sprite,x,y,int(y),FLOWER_FADEOUT_END_Y,FLOWER_FADEOUT_H,0.0,angle);
}

//foreground class
Foreground::Foreground(const SpriteCollection *spr_coll) {
	this->flower_spr=spr_coll->get_sprite("flower");
	this->p1_flower=false;
	this->p2_flower=false;
}

Foreground::~Foreground() {
	for (Flowers::iterator iter=p1_flowers.begin(); iter!=p1_flowers.end(); iter++) delete *iter;
	for (Flowers::iterator iter=p2_flowers.begin(); iter!=p2_flowers.end(); iter++) delete *iter;
}

void Foreground::draw() {
	if (p1_flower) while (p1_flowers.size()<FLOWER_N) p1_flowers.push_back(new Flower(flower_spr,PLAYER_1));
	if (p2_flower) while (p2_flowers.size()<FLOWER_N) p2_flowers.push_back(new Flower(flower_spr,PLAYER_2));
	
	for (Flowers::iterator iter=p1_flowers.begin(); iter!=p1_flowers.end();) {
		if ((*iter)->update()) {
			(*iter)->draw();
			iter++; 
		} else if (p1_flower) {
			(*iter)->reset();
			iter++;
		} else {
			delete *iter;
			iter=p1_flowers.erase(iter);
		}
	}

	for (Flowers::iterator iter=p2_flowers.begin(); iter!=p2_flowers.end();) {
		if ((*iter)->update()) {
			(*iter)->draw();
			iter++; 
		} else if (p2_flower) {
			(*iter)->reset();
			iter++;
		} else {
			delete *iter;
			iter=p2_flowers.erase(iter);
		}
	}
}
