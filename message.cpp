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
#include "message.h"

Message::Message(EVENT event,Unit *receiver,Unit *sender,int delay,PRIORITY priority,int data,float x) {
	this->event=event;
	this->receiver=receiver;
	this->sender=sender;
	this->delay=delay;
	this->priority=priority;
	this->data=data;
	this->x=x;
}

Message Message::decrease_delay() const {
	return Message(event,receiver,sender,delay-1,priority,data);
}

bool MessageLess::operator()(const Message &a,const Message &b) {
	return a.priority<b.priority;
}

std::ostream &operator<<(std::ostream &os,const Message &message) {
	return os<<"message "<<mess_name[message.event]<<" from "<<message.sender<<" to "<<message.receiver<<" delay:"<<message.delay<<" priority:"<<message.priority<<" data:"<<message.data;
}
