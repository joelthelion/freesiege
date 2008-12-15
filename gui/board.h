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
#ifndef __BOARD_H
#define __BOARD_H

#include "combinaisoncollection.h"
#include "battlefield.h"

#define BOARD_H 5  //Board grid height
#define BOARD_W 6  //Board grid width
#define BOARD_BORDER 5
#define ELEM_H  50 //Height of element sprite
#define ELEM_W  50 //Width of element sprite
#define SPACING 10 //Spacing between element

class Board {
public:
	Board(const SpriteCollection *spr_coll,const CombinaisonCollection *com_coll,BattleField *field,PLAYER player);
	~Board();
	void draw();
	void logic();
	int cursor_i,cursor_j;
	int select_i,select_j;
private:
	PLAYER player;
	SDLKey key_select;
	SDLKey key_swap;
	SDLKey key_validate;
	SDLKey key_left;
	SDLKey key_right;
	SDLKey key_down;
	SDLKey key_up;
	
	void draw_background(float offset);
	const SpriteCollection *spr_coll;
	char board[BOARD_H][BOARD_W];
	Sprite *bit_wood;
	Sprite *bit_steel;
	Sprite *bit_magic;
	Sprite *bit_fire;
	Sprite *bit_cursor;

	void compute();
	void compute_match();
	CombinaisonCollection::Coll matched;
	const CombinaisonCollection *com_coll;

	void compute_combi_str();
	std::string combi_str;

	void validate();
	BattleField *field;

	enum STATE {IDLE, SELECTED, VALIDATE};
	STATE state;
	bool state_changed;
	bool cursor_changed;
	
};	

#endif
