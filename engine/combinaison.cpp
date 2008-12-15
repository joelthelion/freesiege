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
#include "combinaison.h"
#include "board.h"

std::ostream &operator<<(std::ostream &os,const Combinaison &combi) {
	return os<<"("<<combi.start_i<<","<<combi.start_j<<")"<<static_cast<std::string>(combi);
}

Combinaison::Combinaison(const std::string &str): std::string(str) { compute_start(); }
Combinaison::Combinaison(const char *str): std::string(str) { compute_start(); }

void Combinaison::compute_start() {
	const std::string::const_iterator end=this->end();
	std::string::const_iterator iter=this->begin();
	int k=0;
	while (iter!=end) {
		if (*iter++==ANY) k++;
		else break;
	}
	this->start_i=k/BOARD_W;
	this->start_j=k%BOARD_W;
}

bool Combinaison::is_valid() const {
	if (this->size()!=2*BOARD_W) return false;

	const std::string::const_iterator end=this->end();
	std::string::const_iterator iter=this->begin();
	while (iter!=end) {
		switch (*iter++) {
		case WOOD:
		case MAGIC:
		case STEEL:
		case FIRE:
		case ANY:
		case NONE:
			break;
		default:
			return false;
		}
	}
	return true;
}

bool Combinaison::element_match(const char a,const char b) {
	if (b==ANY) return true;
	else if (a==ANY) return true;
	else if (a==b && a!=USED) return true;
	else return false;
}

bool Combinaison::match(const Combinaison &combi) const {
	if (!this->is_valid() || !combi.is_valid()) return false;
	const std::string::const_iterator a_end=this->end();
	std::string::const_iterator a_iter=this->begin();
	const std::string::const_iterator b_end=combi.end();
	std::string::const_iterator b_iter=combi.begin();
	while (a_iter!=a_end || b_iter!=b_end) {
		if (!element_match(*a_iter++,*b_iter++)) return false;
	}
	return true;
}

void Combinaison::merge(const Combinaison &combi) {
	const std::string::const_iterator a_end=this->end();
	std::string::iterator a_iter=this->begin();
	const std::string::const_iterator b_end=combi.end();
	std::string::const_iterator b_iter=combi.begin();
	while (a_iter!=a_end || b_iter!=b_end) {
		if (*b_iter!=ANY) *a_iter=USED;
		a_iter++;
		b_iter++;
	}
}
