//	This file is part of freesiege program <http://freesiege.sourceforge.net>.
//	Copyright (C) 2007 Pierre Gueth <pierregueth@users.sourceforge.net>
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
#include "config.h"

#include "musiccollection.h"

MusicCollection::MusicCollection(const std::string &path,const std::string &base_dir,bool sound) {
    this->sound=false;
    if (Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,1024)==-1) {
        std::cerr<<"mixer init failed..."<<Mix_GetError()<<std::endl;
        return;
    }

	std::ifstream file(path.c_str());
	if (file.rdstate()) {
		std::cerr<<"cannot find music config file "<<path<<std::endl;
		return;
	}
    
	std::string line;
	std::string name;
	std::string music_file;
	std::string::size_type pos;
	
	file>>line;
	int c=0;
	Mix_Music *music;
	while (!file.eof()) {
        c++;
		if (line[0] != '#') { //Ignore comment lines
			pos=line.find(":",0);
			if (pos!=std::string::npos) {
				name=line.substr(0,pos);
                music_file=line.substr(pos+1);

                music=Mix_LoadMUS((base_dir+music_file).c_str());
				if (music) {
					coll[name]=music;
				} else std::cerr<<"error loading music "<<music_file<<" in "<<path<<":"<<c<<std::endl;
			} else std::cerr<<"bad line "<<path<<":"<<c<<std::endl; }
		file>>line;
	}

    this->sound=sound;
    Mix_VolumeMusic(MIX_MAX_VOLUME);
}

MusicCollection::~MusicCollection() {
  for (Coll::iterator iter=coll.begin(); iter!=coll.end(); iter++) Mix_FreeMusic(iter->second);
  Mix_CloseAudio();
}

void MusicCollection::play_music(const std::string &name) {
    if (!sound) return;

    Coll::iterator selection=coll.find(name);
    if (selection!=coll.end()) {
        Mix_PlayMusic(selection->second,-1);
    } else std::cerr<<"no music named '"<<name<<"'"<<std::endl;
}

void MusicCollection::play_random_music() {
   if (!sound || coll.empty()) return;

   int selection=rand()%coll.size();
   Coll::const_iterator iter=coll.begin();

   while(selection>0 && iter!=coll.end()) {
     selection--;
     iter++;
   }
   //std::cout<<"playing "<<iter->first<<std::endl;

   Mix_PlayMusic(iter->second,-1);
}

std::ostream &operator<<(std::ostream &os,const MusicCollection &collection) {
    if (!collection.sound) return os<<"sound disabled!!";

    if (collection.coll.empty()) {
      os<<"no musics!!";
      return os;
    }

    os<<collection.coll.size()<<" musics:";

    MusicCollection::Coll::const_iterator iter=collection.coll.begin();
    while (true) {
      os<<iter->first;
      iter++;
      if (iter==collection.coll.end()) break;
      os<<"/";
    }
    os.flush();

    return os;
}
