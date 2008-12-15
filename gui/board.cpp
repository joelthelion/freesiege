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
#include "board.h"

#include "spritecollection.h"
#include "battlefield.h"
#include "combinaison.h"
#include <iostream>

Board::Board(const SpriteCollection *spr_coll,const CombinaisonCollection *com_coll,BattleField *field,PLAYER player) {
	//init player key context
	this->player=player;
	if (player==PLAYER_1) {
		key_select=P1_KEY_SELECT;
		key_swap=P1_KEY_SWAP;
		key_validate=P1_KEY_VALIDATE;
		key_left=P1_KEY_LEFT;
		key_right=P1_KEY_RIGHT;
		key_down=P1_KEY_DOWN;
		key_up=P1_KEY_UP;
	} else {
		key_select=P2_KEY_SELECT;
		key_swap=P2_KEY_SWAP;
		key_validate=P2_KEY_VALIDATE;
		key_left=P2_KEY_LEFT;
		key_right=P2_KEY_RIGHT;
		key_down=P2_KEY_DOWN;
		key_up=P2_KEY_UP;
	}	
		
	//init board randomly
	for (int i=0;i<BOARD_H;i++) {
		for (int j=0;j<BOARD_W;j++) {
			switch (rand()%4) {
			case 0:
				this->board[i][j]=WOOD;
				break;
			case 1:
				this->board[i][j]=FIRE;
				break;
			case 2:
				this->board[i][j]=STEEL;
				break;
			case 3:
			default:
				this->board[i][j]=MAGIC;
			}
		}
	}

	//init sprites stuff
	this->spr_coll=spr_coll;
	this->bit_wood=spr_coll->get_sprite("wood");
	this->bit_steel=spr_coll->get_sprite("steel");
	this->bit_magic=spr_coll->get_sprite("magic");
	this->bit_fire=spr_coll->get_sprite("fire");
	this->bit_cursor=spr_coll->get_sprite("cursor");
	this->cursor_i=0;
	this->cursor_j=0;
	this->select_i=-1;
	this->select_j=-1;
	
	this->state=IDLE;
	this->state_changed=false;
	this->cursor_changed=false;
	
	//init combinaisons stuff
	this->com_coll=com_coll;
	
	//init battle field stuff
	this->field=field;
	compute();
}

void Board::compute() {
	compute_combi_str();
	compute_match();
}

void Board::compute_match() {
	int y=0;
	const CombinaisonCollection::Coll::const_iterator end=this->com_coll->coll.end();
	CombinaisonCollection::Coll::const_iterator iter=this->com_coll->coll.begin();
	Combinaison combi(this->combi_str);
	matched.clear();
	while (iter!=end) {
		if (combi.match(*iter->first)) {
			matched.push_back(*iter);
			combi.merge(*iter->first);
		}
		y+=10;
		iter++;
	}
}

void Board::compute_combi_str() {
	combi_str="";
	for (int i=BOARD_H-2; i<BOARD_H; i++) {
		for (int j=0;j<BOARD_W;j++) {
			combi_str+=this->board[i][j];
		}
	}
}

void Board::draw_background(float offset) {
	//draw elements
	float x,y;
	y=SPACING/2+BOARD_BORDER;
	for (int i=0;i<BOARD_H;i++) {
		x=SPACING/2+offset;
		for (int j=0;j<BOARD_W;j++) {
			switch (this->board[i][j]) {
			case WOOD:
				bit_wood->draw(x,y);
				break;
			case MAGIC:
				bit_magic->draw(x,y);
				break;
			case STEEL:
				bit_steel->draw(x,y);
				break;
			case FIRE:
				bit_fire->draw(x,y);
				break;
			default:
				break;
			}
			x+=ELEM_W+SPACING;
		}
		y+=ELEM_H+SPACING;
	}
	
	//draw matched sprite
	Combinaison *combi;
	for (CombinaisonCollection::Coll::const_iterator iter=matched.begin(); iter!=matched.end(); iter++) {
		combi=iter->first;
		spr_coll->get_sprite(*iter->second)->draw(SPACING/2+offset+combi->start_j*(ELEM_W+SPACING),SPACING/2+BOARD_BORDER+(combi->start_i+BOARD_H-2)*(ELEM_H+SPACING));
	}
}

void Board::validate() {
	if (!matched.empty()) {
		//out the created unit
		const CombinaisonCollection::Coll::const_iterator end=this->matched.end();
		CombinaisonCollection::Coll::const_iterator iter=this->matched.begin();
		//std::cout<<"spawning for player"<<player<<":";
		while (iter!=end) {
			//std::cout<<" "<<*iter->second;
			std::string *name=iter->second;
			if (*name=="soldier") field->spawn(SOLDIER,player);
			else if (*name=="druid") field->spawn(DRUID,player);
			else if (*name=="knight") field->spawn(KNIGHT,player);
			else if (*name=="golem") field->spawn(GOLEM,player);
			else if (*name=="plant") field->spawn(PLANT,player);
			else if (*name=="dragon") field->spawn(DRAGON,player);
			else if (*name=="marion") field->spawn(FLOWER,player);
			else if (*name=="veteran") field->spawn(VETERAN,player);
			else {
              std::cout<<"WARNING default spawing..."<<std::endl;
              field->spawn(SOLDIER,player);
            }
			iter++;
		}
		//std::cout<<std::endl;

		//scrool board and ...
		for (int i=BOARD_H-1;i>=2;i--) {
			for (int j=0;j<BOARD_W;j++) {
				this->board[i][j]=this->board[i-2][j];
			}
		}
		//generate two line randomly
		for (int i=0;i<2;i++) {
			for (int j=0;j<BOARD_W;j++) {
				switch (rand()%4) {
				case 0:
					this->board[i][j]=WOOD;
					break;
				case 1:
					this->board[i][j]=FIRE;
					break;
				case 2:
					this->board[i][j]=STEEL;
					break;
				case 3:
				default:
					this->board[i][j]=MAGIC;
				}
			}
		}
		
		//put board up to date
		compute();
	} //else std::cout<<"nothing to validate for player"<<player<<" ..."<<std::endl;
}

void Board::logic() {
	Uint8 *key=SDL_GetKeyState(NULL);

	if (!this->cursor_changed) {
		if (key[key_left]) this->cursor_j--;
		else if (key[key_right]) this->cursor_j++;
		if (key[key_up]) this->cursor_i--;
		else if (key[key_down]) this->cursor_i++;
		this->cursor_changed=key[key_left] || key[key_right] || key[key_down] || key[key_up];
	}
	
	if (this->cursor_i>=BOARD_H) this->cursor_i=0;
	else if (this->cursor_i<0) this->cursor_i=BOARD_H-1;
	if (this->cursor_j>=BOARD_W) this->cursor_j=0;
	else if (this->cursor_j<0) this->cursor_j=BOARD_W-1;
	
	char dummy;

	switch (this->state) {
	case IDLE:
		if (key[key_select] && !this->state_changed) {
			this->select_i=this->cursor_i;
			this->select_j=this->cursor_j;
			this->state=SELECTED;
			this->state_changed=true;
		} else if (key[key_validate] && !this->state_changed) {
			this->state=VALIDATE;
			this->state_changed=true;
		}
		break;
	case SELECTED:
		if (key[key_swap] && !this->state_changed) {
			dummy=this->board[cursor_i][cursor_j];
			this->board[cursor_i][cursor_j]=this->board[select_i][select_j];
			this->board[select_i][select_j]=dummy;
			compute();
			this->state=IDLE;
			this->state_changed=true;
		} else if (key[key_validate] && !this->state_changed) {
			this->state=VALIDATE;
			this->state_changed=true;
		}
		break;
	case VALIDATE:
		if (!this->state_changed) {
			validate();
			this->state=IDLE;
			this->state_changed=true;
		}
		break;
	default:
		std::cerr<<"unknown state: "<<this->state<<std::endl;
		break;
	}
}

Board::~Board() {}

void Board::draw() {
	Uint8 *key=SDL_GetKeyState(NULL);
	
	int offset;
	if (player==PLAYER_1) offset=BOARD_BORDER;
	else offset=SCREEN_W-BOARD_W*(ELEM_H+SPACING)-SPACING-BOARD_BORDER;
	
	draw_background(offset);
	bit_cursor->draw(offset+this->cursor_j*(ELEM_W+SPACING),BOARD_BORDER+this->cursor_i*(ELEM_H+SPACING));
	if (this->state==SELECTED) bit_cursor->draw(offset+this->select_j*(ELEM_W+SPACING),BOARD_BORDER+this->select_i*(ELEM_H+SPACING));
	
	this->state_changed=this->state_changed && (key[key_select] || key[key_swap] || key[key_validate]); //reset state changed
	this->cursor_changed=this->cursor_changed && (key[key_left] || key[key_right] || key[key_down] || key[key_up]); //reset cursor changed
}
