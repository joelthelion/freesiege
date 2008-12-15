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
#include "spritecollection.h"

SpriteCollection::SpriteCollection(const std::string &spr_path,const std::string &anim_path,const std::string &base_dir,TextureIds ids) {
	load_sprites(spr_path,base_dir,ids);
	load_anim(anim_path,base_dir,ids+100);
}

void SpriteCollection::load_sprites(const std::string &path,const std::string &base_dir,TextureIds ids) {
	//loading sprites
	std::ifstream file(path.c_str());
	if (file.rdstate()) {
		std::cerr<<"cannot find sprites config file "<<path<<std::endl;
		return;
	}

	std::string line;
	std::string name;
	std::string sprite_file;
	std::string::size_type pos;
	
	file>>line;
	int c=0;
	SDL_Surface *sprite;
	while (!file.eof()) {
		if (line[0] != '#') { //Ignore comment lines
			c++;
			pos=line.find(":",0);
			if (pos!=std::string::npos) {
				name=line.substr(0,pos);
				sprite_file=line.substr(pos+1);

				sprite=load_surface(base_dir+sprite_file);
				if (sprite) {
					spr_coll[name]=new Sprite(sprite,*ids++);
				} else std::cerr<<"error loading sprite"<<sprite_file<<" in "<<path<<":"<<c<<std::endl;
			} else std::cerr<<"bad line "<<path<<":"<<c<<std::endl; }
		file>>line;
	}
}

void SpriteCollection::load_anim(const std::string &path,const std::string &base_dir,TextureIds ids) {
	//loading anims
	std::ifstream file(path.c_str());
	if (file.rdstate()) {
		std::cerr<<"cannot find anims config file "<<path<<std::endl;
		return;
	}

	std::string line;
	std::string name;
	std::string anim_prefix;
	std::string anim_ext;
	int n_frames;
	std::string::size_type pos;
	
	file>>line;
	int c=0;
	Anim *anim;
	while (!file.eof()) {
		c++;

		//name
		pos=line.find(":",0);
		if (pos==std::string::npos) {
			std::cerr<<"bad name line "<<path<<":"<<c<<std::endl;
			break;
		}
		name=line.substr(0,pos);
		line=line.substr(pos+1);

		//nframes
		pos=line.find(":",0);
		if (pos==std::string::npos) {
			std::cerr<<"bad nframes line "<<path<<":"<<c<<std::endl;
			break;
		}
		n_frames=atoi(line.substr(0,pos).c_str());
		line=line.substr(pos+1);

		//prefix
		pos=line.find(":",0);
		if (pos==std::string::npos) {
			std::cerr<<"bad anim prefix line "<<path<<":"<<c<<std::endl;
			break;
		}
		anim_prefix=line.substr(0,pos);

		//extension
		anim_ext=line.substr(pos+1);
		
		anim=new Anim(base_dir+anim_prefix,n_frames,anim_ext,ids);
		if (anim->is_valid()) {
			anim_coll[name]=anim;
			ids+=anim->size();
		}
		else delete anim;
		file>>line;
	}
}

SpriteCollection::~SpriteCollection() {
	for (SprColl::iterator iter=spr_coll.begin(); iter!=spr_coll.end(); iter++) delete iter->second;
	for (AnimColl::iterator iter=anim_coll.begin(); iter!=anim_coll.end(); iter++) delete iter->second;
}

Sprite *SpriteCollection::get_sprite(const std::string &str) const {
	SprColl::const_iterator res=spr_coll.find(str);
	if (res!=spr_coll.end()) return res->second;
	else {
		std::cerr<<"requesting non collection sprite "<<str<<std::endl;
		return NULL;
	}
}

Anim::CycleIterator SpriteCollection::get_anim_cycle_iterator(const std::string &str,float speed) const {
	AnimColl::const_iterator res=anim_coll.find(str);
	if (res!=anim_coll.end()) return res->second->get_cycle_iterator(speed);
	else {
		std::cerr<<"requesting non collection anim "<<str<<std::endl;
		return Anim::CycleIterator();
	}
}

Anim::ForwardBackwardIterator SpriteCollection::get_anim_forward_backward_iterator(const std::string &str,float speed) const {
	AnimColl::const_iterator res=anim_coll.find(str);
	if (res!=anim_coll.end()) return res->second->get_forward_backward_iterator(speed);
	else {
		std::cerr<<"requesting non collection anim "<<str<<std::endl;
		return Anim::ForwardBackwardIterator();
	}
}

std::ostream &operator<<(std::ostream &os,const SpriteCollection &collection) {
	if (!collection.anim_coll.empty()) { 
		SpriteCollection::AnimColl::const_iterator anim_iter=collection.anim_coll.begin();
		os<<collection.anim_coll.size()<<" anims:"<<anim_iter->first<<"("<<anim_iter->second->size()<<")";
		anim_iter++;
		while (anim_iter!=collection.anim_coll.end()) {
			os<<"/"<<anim_iter->first<<"("<<anim_iter->second->size()<<")";
			anim_iter++;
		}
	} else os<<"no anims!!!";
	os<<std::endl;

	if (!collection.spr_coll.empty()) {
		SpriteCollection::SprColl::const_iterator spr_iter=collection.spr_coll.begin();
		os<<collection.spr_coll.size()<<" sprites:"<<spr_iter++->first;
		while (spr_iter!=collection.spr_coll.end()) os<<"/"<<spr_iter++->first;
	} else os<<"no sprites!!!";

	return os;
}
