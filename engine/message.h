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
#ifndef __MESSAGE_H
#define __MESSAGE_H

#include <vector>
#include <queue>
#include <iostream>
#include <string>
class Unit;

const std::string mess_name[10]={"collision","attack","walk","die","damage player","fight ended","trigger","shoot","spawn","null"};

#define NO_DELAY 0

class Message {
friend std::ostream &operator<<(std::ostream &os,const Message &message);
public:
	typedef enum {PRIORITY_LOWEST,PRIORITY_LOW,PRIORITY_NORMAL,PRIORITY_HIGH} PRIORITY;
	typedef enum {EVENT_COLLISION,EVENT_ATTACK,EVENT_WALK,EVENT_DIE,EVENT_DAMAGE_PLAYER,
		EVENT_ENDFIGHT,EVENT_TRIGGER,EVENT_SHOOT,EVENT_SPAWN,EVENT_NULL,EVENT_ENDFLOWER} EVENT;
	
	Message(EVENT event=EVENT_NULL,Unit *receiver=NULL,Unit *sender=NULL,int delay=0,PRIORITY priority=PRIORITY_NORMAL,int data=0,float x=0);
	Message decrease_delay() const;

	Unit *sender;
	Unit *receiver;
	int delay;
	PRIORITY priority;
	EVENT event;
	int data;
	float x;
};

class MessageLess {
public:
	bool operator()(const Message &a,const Message &b);
};

typedef std::priority_queue<Message,std::vector<Message>,MessageLess> MessageQueue;

#endif
