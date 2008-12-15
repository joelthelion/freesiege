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
#include "anim.h"

#include <iostream>
#include <sstream>
#include "utils.h"


//Cycle iterator
Anim::CycleIterator::CycleIterator(const SpriteVector *vect,float speed) {
	this->speed=speed;
	this->position=0.0;
	this->vect=vect;
	if (this->vect!=NULL) this->iter=this->vect->begin();
}

Sprite *Anim::CycleIterator::get_next_bitmap() {
	Sprite *current=*iter;
	position+=speed;
	while (position>=1.0) {
		position-=1.0;
		iter++;
		if (iter==vect->end()) iter=vect->begin();
	}
	return current;
}

void Anim::CycleIterator::set_speed(float speed) { this->speed = speed; }
bool Anim::CycleIterator::is_valid() const { return vect!=NULL; }

//Forward backward iterator
Anim::ForwardBackwardIterator::ForwardBackwardIterator(const SpriteVector *vect,float speed) {
	this->speed=speed;
	this->position=0.0;
	this->vect=vect;
	this->forward=true;
	if (this->vect!=NULL) {
		this->iter_forward=this->vect->begin();
		this->iter_backward=this->vect->rbegin();
	}
}

Sprite *Anim::ForwardBackwardIterator::get_next_bitmap() {
	Sprite *current;
	if (forward) current=*iter_forward;
	else current=*iter_backward;
	position+=speed;
	while (position>=1.0) {
		position-=1.0;
		if (forward) {
			iter_forward++;
			if (iter_forward==vect->end()) {
				forward=false;
				iter_forward=vect->begin();
			}
		} else {
			iter_backward++;
			if (iter_backward==vect->rend()) {
				forward=true;
				iter_backward=vect->rbegin();
			}
		}
	}
	return current;
}
		
void Anim::ForwardBackwardIterator::set_speed(float speed) { this->speed = speed; }
bool Anim::ForwardBackwardIterator::is_valid() const { return vect!=NULL; }

//Anim
Anim::Anim(const std::string &path,int n_frames,const std::string &ext,TextureIds first_id) {
	vect=new SpriteVector;
	valid=true;

	std::ostringstream full_path;
	SDL_Surface *frame;
	Sprite *sprite;

	for (int n=0; n<n_frames; n++) {
		full_path.str("");
		full_path<<path;
		if (n<=9) full_path<<"0";
		full_path<<n<<"."<<ext;

		frame=load_surface(full_path.str());
		if (frame) {
			sprite=new Sprite(frame,*first_id++);
			SDL_FreeSurface(frame);
			vect->push_back(sprite);
		} else {
			valid=false;
			std::cerr<<"error loading anim frame "<<full_path.str()<<std::endl;
		}
	}
}

Anim::~Anim() {}

Anim::SpriteVector::size_type Anim::size() const { return vect->size(); }
bool Anim::is_valid() const { return valid; }

Anim::CycleIterator Anim::get_cycle_iterator(float speed) const { return CycleIterator(vect,speed); }
Anim::ForwardBackwardIterator Anim::get_forward_backward_iterator(float speed) const { return ForwardBackwardIterator(vect,speed); }
