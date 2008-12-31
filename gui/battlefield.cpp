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
#include "battlefield.h"

#include "collisionmatrix.h"
#include "param.h"

//#include "unit.h"
#include "plant.h" //for resetting plant counter

#define FIELD_UPDATE_FRAME_SKIP 1 //skip UPDATE_FRAME_SKIP between update

BattleField::BattleField(const SpriteCollection *spr_coll,LifeBar *lifebar1,LifeBar *lifebar2,Foreground * foreground) {
	this->spr_coll=spr_coll;
	this->lifebar1=lifebar1;
	this->lifebar2=lifebar2;

	bit_castle=spr_coll->get_sprite("castle");

	current_mess_queue=new MessageQueue;

	frame_skip_count=0;
	Plant::reset_hasplant(); //Reset plant counters
	this->foreground = foreground;
}

BattleField::~BattleField() {
	delete current_mess_queue;

	const Units::const_iterator end=units.end();
	Units::const_iterator iter=units.begin();
	while (iter!=end) delete *iter++;
}

void BattleField::spawn(UNIT_ID name,PLAYER player,float x) {
	switch(name)
	{
		case FLOWER:
			if (player==PLAYER_1 && !foreground->p2_flower) {
				foreground->p2_flower = true;
				current_mess_queue->push(Message(Message::EVENT_ENDFLOWER,0,0,100,Message::PRIORITY_NORMAL,PLAYER_2));
			} else if (player==PLAYER_2 && !foreground->p1_flower) {
				foreground->p1_flower = true;
				current_mess_queue->push(Message(Message::EVENT_ENDFLOWER,0,0,100,Message::PRIORITY_NORMAL,PLAYER_1));
			}
			break;
		default:
			Unit *unit=Unit::spawn(name,this->spr_coll,player,x);
			if (unit) units.push_front(unit);
	}
}

int BattleField::get_unit_count(PLAYER player)
{
    int count=0;
    for (Units::iterator i = units.begin();i!=units.end();i++)
        if ((*i)->get_player() == player) count++;
    return count;
}

int BattleField::get_nonplant_unit_count(PLAYER player)
{
    int count=0;
    for (Units::iterator i = units.begin();i!=units.end();i++)
        if ((*i)->get_player() == player && (*i)->name != "plant") count++;
    return count;
}

void BattleField::draw() {
	if (++frame_skip_count>FIELD_UPDATE_FRAME_SKIP) {
		//post collision event
		CollisionMatrix coll_matrix(units);
		coll_matrix.post_message(current_mess_queue);

		//let unit post their event
		for (Units::const_iterator iter=units.begin(); iter!=units.end(); iter++) (*iter)->post_message(current_mess_queue);

		//parse event
		MessageQueue *next_mess_queue=new MessageQueue;
		Message mess;
		//if (!current_mess_queue->empty()) std::cout<<"---------"<<current_mess_queue->size()<<std::endl;
		while (!current_mess_queue->empty()) {
				mess=current_mess_queue->top();
				if (mess.delay==0) {
					//std::cout<<"processing "<<mess<<std::endl;
					if (mess.event==Message::EVENT_ENDFLOWER) {
							if (mess.data == PLAYER_1)
								foreground->p1_flower=false;
							else
								foreground->p2_flower=false;
						}
					if (std::find(units.begin(),units.end(),mess.receiver)!=units.end()) {
						if (mess.event==Message::EVENT_DAMAGE_PLAYER) {
							if (mess.receiver->get_player()==PLAYER_1) lifebar2->damage(mess.data);
							else if (mess.receiver->get_player()==PLAYER_2) lifebar1->damage(mess.data);
						} else if (mess.event==Message::EVENT_SPAWN) {
							spawn(static_cast<UNIT_ID>(mess.data),mess.receiver->get_player(),mess.x);
						} 
						mess.receiver->handle_message(mess,next_mess_queue);
					}
				} else {
					next_mess_queue->push(mess.decrease_delay());
					//std::cout<<"delaying "<<next_mess_queue->top()<<std::endl;
				}
				current_mess_queue->pop();
		}
		
		//swap message queue
		delete current_mess_queue;
		current_mess_queue=next_mess_queue;

		//clean up dead units
		for (Units::iterator iter=units.begin(); iter!=units.end();) {
			if ((*iter)->is_dead()) {
				delete *iter;
				iter=units.erase(iter);
			} else iter++;
		}
		
	}
	//draw
	for (Units::const_iterator iter=units.begin(); iter!=units.end(); iter++) (*iter)->draw();

	//draw castles
	bit_castle->draw(0,SCREEN_H-CastleH);
	bit_castle->draw_flip_h(SCREEN_W-bit_castle->w,SCREEN_H-CastleH);
}
