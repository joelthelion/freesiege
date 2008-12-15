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
#include "combinaisoncollection.h"

#include "combinaison.h"
#include <fstream>
#include <iostream>
#include <map>

CombinaisonCollection::CombinaisonCollection(const std::string &path) {
	std::ifstream file(path.c_str());
	if (file.rdstate()) {
		std::cerr<<"cannot find combinaisons config file "<<path<<std::endl;
		return;
	}

	std::string line;
	std::string *name;
	std::string::size_type pos;
	
	file>>line;
	while (!file.eof()) {
		pos=line.find(":",1);
		if (pos!=std::string::npos) {
			name=new std::string(line.substr(0,pos));
			names.push_back(name);

			line=line.substr(pos);
			parse_combinaisons(line,name);
		} else std::cerr<<"bad combinaison line: "<<line<<std::endl;
		file>>line;
	}
}

void CombinaisonCollection::parse_combinaisons(std::string line,std::string *name) {
	Combinaison *combi;
	std::string::size_type pos;

	pos=line.find("/",1);
	while (pos!=std::string::npos) {
		combi=new Combinaison(line.substr(1,pos-1));
		line=line.substr(pos);
		if (combi->is_valid()) {
			coll.push_back(Pair(combi,name));
		} else {
			std::cerr<<"bad combinaison: "<<*combi<<std::endl;
			delete combi;
		}
		pos=line.find("/",1);
	}
}

CombinaisonCollection::~CombinaisonCollection() {
	const Coll::const_iterator coll_end=this->coll.end();
	Coll::const_iterator coll_iter=this->coll.begin();
	while (coll_iter!=coll_end) delete coll_iter++->first;

	const Names::const_iterator names_end=this->names.end();
	Names::const_iterator names_iter=this->names.begin();
	while (names_iter!=names_end) delete *names_iter++;
}

std::ostream &operator<<(std::ostream &os,const CombinaisonCollection &collection) {
	typedef std::map<std::string*,int> Count;
	Count count;

	//initialize combinaisons count
	const CombinaisonCollection::Names::const_iterator names_end=collection.names.end();
	CombinaisonCollection::Names::const_iterator names_iter=collection.names.begin();
	while (names_iter!=names_end) count[*names_iter++]=0;
	
	//count corresponding combinaisons
	const CombinaisonCollection::Coll::const_iterator coll_end=collection.coll.end();
	CombinaisonCollection::Coll::const_iterator coll_iter=collection.coll.begin();
	while (coll_iter!=coll_end) count[coll_iter++->second]++;

	//display results
	const Count::const_iterator count_end=count.end();
	Count::const_iterator count_iter=count.begin();
	os<<count.size()<<" units:"<<*count_iter++->first<<"("<<count_iter->second<<" combis)";
	while (count_iter!=count_end) os<<"/"<<*count_iter++->first<<"("<<count_iter->second<<" combis)";
	
	return os;
}

