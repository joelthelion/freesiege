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
#ifndef __COLLISIONMATRIX_H
#define __COLLISIONMATRIX_H

#include <list>
#include <map>
#include <iostream>
#include <vector>
#include "message.h"
class Unit;


class CollisionMatrix {
friend std::ostream &operator<<(std::ostream &os,const CollisionMatrix &coll_matrix);
public:
	typedef std::list<Unit*> Units;
	
	CollisionMatrix(const Units &units);
	~CollisionMatrix();
	
	bool collide( Unit* unit_a, Unit* unit_b) const; //FIXME on a fait cette classe pour eviter ca mais pour les tests c'est plus agreable
	int get(int id_unit_a,int id_unit_b) const;
	void post_message(MessageQueue *mess_queue) const;
private:

	void increment(int id_unit_a,int id_unit_b);

	typedef std::map<Unit*,int> Ids;
	Ids ids;
	typedef std::vector<Unit*> UnitsVector;
	UnitsVector units_vector;
	
	typedef int** Matrix;
	Matrix matrix;
};

#endif
