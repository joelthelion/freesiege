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
#ifndef __SPRITE__COLLECTION_H
#define __SPRITE__COLLECTION_H

#include <map>
#include "anim.h"

class SpriteCollection {
friend std::ostream &operator<<(std::ostream &os,const SpriteCollection &collection);
public:
	typedef std::map<std::string,Sprite*> SprColl;
	typedef std::map<std::string,Anim*> AnimColl;	
	SpriteCollection(const std::string &spr_path,const std::string &anim_path,const std::string &base_dir,TextureIds ids);
	~SpriteCollection();
	Sprite *get_sprite(const std::string &str) const;
	Anim::CycleIterator get_anim_cycle_iterator(const std::string &str,float speed) const;
	Anim::ForwardBackwardIterator get_anim_forward_backward_iterator(const std::string &str,float speed) const;
private:
	typedef std::map<std::string,SDL_Surface*> SurfColl;
	void load_sprites(const std::string &path,const std::string &base_dir,TextureIds ids);
	void load_anim(const std::string &path,const std::string &base_dir,TextureIds ids);
	SprColl spr_coll;
	AnimColl anim_coll;
};

std::ostream &operator<<(std::ostream &os,const SpriteCollection &collection);

#endif
