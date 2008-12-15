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
#ifndef __ANIM_H
#define __ANIM_H

#include "sprite.h"

class Anim {
public:
	typedef std::vector<Sprite*> SpriteVector;

	class CycleIterator {
	public:
		CycleIterator(const SpriteVector *vect=NULL,float speed=1.0);
		Sprite *get_next_bitmap();
		bool is_valid() const;
		void set_speed(float);
	private:
		float speed;
		float position;
		const SpriteVector *vect;
		SpriteVector::const_iterator iter;
	};

	class ForwardBackwardIterator {
	public:
		ForwardBackwardIterator(const SpriteVector *vect=NULL,float speed=1.0);
		Sprite *get_next_bitmap();
		bool is_valid() const;
		void set_speed(float);
	private:
		float speed;
		float position;
		const SpriteVector *vect;
		bool forward;
		SpriteVector::const_iterator iter_forward;
		SpriteVector::const_reverse_iterator iter_backward;
	};

	Anim(const std::string &path,int n_frames,const std::string &ext,TextureIds first_id);
	~Anim();
	CycleIterator get_cycle_iterator(float speed) const;
	ForwardBackwardIterator get_forward_backward_iterator(float speed) const;
	SpriteVector::size_type size() const;
	bool is_valid() const;
private:
	SpriteVector *vect;
	bool valid;
};

#endif
