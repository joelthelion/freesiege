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
#include "background.h"

#include "utils.h" //for factor
#include "param.h"

#define N_CLOUD 10
#define MIN_CLOUD_SPEED 4 
#define MAX_CLOUD_ALTITUDE 300
#define BACK_UPDATE_FRAME_SKIP 2 


Background::Background(const SpriteCollection *spr_coll) {
	bit_back=spr_coll->get_sprite("background_back");
	bit_front=spr_coll->get_sprite("background_front");
	bit_sun=spr_coll->get_sprite("sun");
	bit_cloud=spr_coll->get_sprite("cloud");
	
	frame_skip_count=0;
	
	//generate clouds
	Cloud *cloud;
	for (int i=0; i<N_CLOUD; i++) {
		cloud=new Cloud;
		cloud->x=rand()%(SCREEN_W+2*int(bit_cloud->w))-int(bit_cloud->w);
		cloud->y=rand()%MAX_CLOUD_ALTITUDE;
		cloud->dx=MIN_CLOUD_SPEED*factor(0.2);
		cloud->alpha=factor(0.6);
		clouds.push_back(cloud);
	}
}

Background::~Background() {
	//delete clouds
	const Clouds::const_iterator end=clouds.end();
	Clouds::const_iterator iter=clouds.begin();
	while (iter!=end) delete *iter++;
}

void Background::compute_clouds() {
	const Clouds::const_iterator end=clouds.end();
	Clouds::const_iterator iter=clouds.begin();
	while (iter!=end) {
		(*iter)->x+=(*iter)->dx;
		if ((*iter)->x>SCREEN_W) {
			(*iter)->x-=SCREEN_W+bit_cloud->w;
			(*iter)->y=rand()%MAX_CLOUD_ALTITUDE;
			(*iter)->dx=MIN_CLOUD_SPEED*factor(0.5);
			(*iter)->alpha=factor(0.6);
		}
		iter++;
	}
}

void Background::draw() {
	bit_back->draw(0,0);
	bit_sun->draw(320,60);
	for (Clouds::const_iterator iter=clouds.begin(); iter!=clouds.end(); iter++) bit_cloud->draw((*iter)->x,(*iter)->y,(*iter)->alpha);
	bit_front->draw(0,0);

	if (++frame_skip_count>BACK_UPDATE_FRAME_SKIP) {
		compute_clouds();
		frame_skip_count=0;
	}
}

