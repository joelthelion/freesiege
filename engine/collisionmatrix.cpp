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
#include "collisionmatrix.h"

#include "unit.h"

typedef std::pair<float,int> CoordPair;// fst: coord, scd: unit index

bool less_than(const CoordPair& p1,const CoordPair& p2) {
	return p1.first < p2.first;
}

CollisionMatrix::CollisionMatrix(const Units &units) {
	unsigned int l;

	typedef std::vector<CoordPair> CoordList; 
	CoordList listx;
	CoordList listy;

	const Units::const_iterator units_end=units.end();
	Units::const_iterator units_iter=units.begin();
	l=0;
	Unit *unit;
	while (units_iter!=units_end) {
		unit=*units_iter;
		if (unit->can_collide()) {
			listx.push_back(CoordPair(unit->x,l));
			listx.push_back(CoordPair(unit->x+unit->w,l));
			listy.push_back(CoordPair(unit->y,l));
			listy.push_back(CoordPair(unit->y+unit->h,l));
			ids[unit]=l;
			this->units_vector.push_back(unit);
			l++;
		}
		units_iter++;
	}
	
	assert(units_vector.size()==ids.size());
	
	unsigned int n=ids.size();
	matrix=new int*[n];	
	for (unsigned int k=0; k<n; k++) { 		//FIXME : l wasted bytes in memory
		matrix[k]=new int[k]; 			//biggest index first, smallest second
		std::fill(matrix[k],matrix[k]+k,0);
	}

	// Teh aLGorItHM
	//FIXME strange incrementation of lower right diaonal element...

	std::sort(listx.begin(),listx.end(),less_than);
	std::sort(listy.begin(),listy.end(),less_than);	

	typedef std::list<int> Stack;
	Stack current;
	const CoordList::const_iterator listx_end = listx.end();
	CoordList::const_iterator listx_iter = listx.begin();
	l=0;
	while (listx_iter != listx_end) {
		Stack::const_iterator index = std::find(current.begin(),current.end(),listx_iter->second);
		if (index == current.end()) current.push_back(listx_iter->second); //powerful lazy implementation
		else {
			current.remove(listx_iter->second);
			for (std::list<int>::const_iterator c_iter = current.begin();c_iter != current.end();c_iter++)
				increment(listx_iter->second,*c_iter);
		}
		listx_iter++;
		l++;
	}

	const CoordList::const_iterator listy_end = listy.end();
	CoordList::const_iterator listy_iter = listy.begin();
	while (listy_iter != listy_end)
	{
		std::list<int>::const_iterator index = std::find(current.begin(),current.end(),listy_iter->second);
		if (index == current.end()) current.push_back(listy_iter->second); //powerful lazy implementation
		else {
			current.remove(listy_iter->second);
			for (std::list<int>::const_iterator c_iter = current.begin();c_iter != current.end();c_iter++)
				increment(listy_iter->second,*c_iter);
		}
		listy_iter++;
	}
}

CollisionMatrix::~CollisionMatrix() {
	int l=ids.size();
	
	for (int k=0;k<l;k++) delete [] matrix[k];
	delete [] matrix;
}


void CollisionMatrix::increment(int id_unit_a,int id_unit_b) {
	if (id_unit_a < id_unit_b)
		matrix[id_unit_b][id_unit_a]++;
	else
		matrix[id_unit_a][id_unit_b]++;
}

int CollisionMatrix::get(int id_unit_a,int id_unit_b) const {
	if (id_unit_a < id_unit_b)
		return matrix[id_unit_b][id_unit_a];
	else
		return matrix[id_unit_a][id_unit_b];
}

bool CollisionMatrix::collide( Unit *unit_a, Unit *unit_b) const {
	int id_unit_a = ids.find(unit_a)->second;
	int id_unit_b = ids.find(unit_b)->second;
	if (id_unit_a < id_unit_b)
		return matrix[id_unit_b][id_unit_a]==2;
	else
		return matrix[id_unit_a][id_unit_b]==2;
}

void CollisionMatrix::post_message(MessageQueue *mess_queue) const {
	const int n=ids.size();
	Unit *unit_a;
	Unit *unit_b;
	for (int k=0;k<n; k++) {
		for (int l=k+1; l<n; l++) {
			if (get(k,l)==2) {
				unit_a=units_vector[k];
				unit_b=units_vector[l];
				mess_queue->push(Message(Message::EVENT_COLLISION,unit_a,unit_b));
				mess_queue->push(Message(Message::EVENT_COLLISION,unit_b,unit_a));
			}
		}
	}				
}

std::ostream &operator<<(std::ostream &os,const CollisionMatrix &coll_matrix) {
	unsigned int s=coll_matrix.ids.size();
	for (unsigned int k=0; k<s; k++) {
		for (unsigned int l=0; l<s; l++) {
			os<<(coll_matrix.get(k,l)==2)<<" ";
		}
		os<<std::endl;
	}
	return os;
}
