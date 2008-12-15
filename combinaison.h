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
#ifndef __COMBINAISON_H
#define __COMBINAISON_H

#include "param.h"
#include <string>

class Combinaison: public std::string {
friend std::ostream &operator<<(std::ostream &os,const Combinaison &combi);
public:
	Combinaison(const Combinaison &); //not implemented
	Combinaison &operator=(const Combinaison &); //not implemented
	
	Combinaison(const std::string &str);
	Combinaison(const char *str);
	//Combinaison(Element *elems[BOARD_H][BOARD_W]);
	bool is_valid() const;
	bool match(const Combinaison &combi) const;
	void merge(const Combinaison &combi);

	int start_i;
	int start_j;
private:
	void compute_start();
	static bool element_match(const char a,const char b);
};

#endif
