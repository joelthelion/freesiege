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
#include "lifebar.h"
#include "param.h"

#define LIFE_Y 320
#define LIFE_INNER_BORDER 3 
#define LIFE_W 360
#define LIFE_H 39
#define LIFE_BORDER 5
#define LIFE_DECREASE 0.002

LifeBar::LifeBar(const SpriteCollection *spr_coll,PLAYER player) {
	this->life=1.0;
	this->old_life=1.0;
	this->player=player;
	border=spr_coll->get_sprite("lifebar_border");
	compute();
}

LifeBar::~LifeBar() {}

void LifeBar::compute() {
	if (life>0) {
		int w=int(life*(LIFE_W-2*LIFE_INNER_BORDER));
		if (player==PLAYER_2) {
			fill_rect_opengl(LIFE_INNER_BORDER,LIFE_INNER_BORDER,w,LIFE_H-2*LIFE_INNER_BORDER,0,1,0);
		} else {
			fill_rect_opengl(LIFE_W-LIFE_INNER_BORDER-w,LIFE_INNER_BORDER,w,LIFE_H-2*LIFE_INNER_BORDER,0,1,0);
		}
	}
}

void LifeBar::damage(int a) {
	life-=float(a)/LIFE_FACTOR;
	if (life<0) life=0;
}

int LifeBar::get_life() const { return int(life*LIFE_FACTOR); }

void LifeBar::draw() {
	if (recov_life>life) recov_life-=LIFE_DECREASE;
	if (recov_life<life) recov_life=life;

	int reco_w=int(recov_life*(LIFE_W-2*LIFE_INNER_BORDER));
	int life_w=int(life*(LIFE_W-2*LIFE_INNER_BORDER));
	if (player==PLAYER_2) {
		fill_rect_opengl(SCREEN_W-LIFE_W+LIFE_INNER_BORDER-LIFE_BORDER,LIFE_Y+LIFE_INNER_BORDER,reco_w,LIFE_H-2*LIFE_INNER_BORDER,1,0,0);
		fill_rect_opengl(SCREEN_W-LIFE_W+LIFE_INNER_BORDER-LIFE_BORDER,LIFE_Y+LIFE_INNER_BORDER,life_w,LIFE_H-2*LIFE_INNER_BORDER,0,1,0);
		border->draw(SCREEN_W-LIFE_W-LIFE_BORDER,LIFE_Y);
	} else {
		fill_rect_opengl(LIFE_W-LIFE_INNER_BORDER-reco_w+LIFE_BORDER,LIFE_Y+LIFE_INNER_BORDER,reco_w,LIFE_H-2*LIFE_INNER_BORDER,1,0,0);
		fill_rect_opengl(LIFE_W-LIFE_INNER_BORDER-life_w+LIFE_BORDER,LIFE_Y+LIFE_INNER_BORDER,life_w,LIFE_H-2*LIFE_INNER_BORDER,0,1,0);
		border->draw(LIFE_BORDER,LIFE_Y);
	}

}
