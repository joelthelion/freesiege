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
#include "utils.h"

#include <sstream>
#include <ctime>

// written by Ste Cork, free for any and all use.
// and patched by us

struct RomanDigit {
	std::string roman;
	int decimal;
};

const RomanDigit roman_digits[]= {
    {"M",  1000},
    {"CM",  900},
    {"D",   500},
    {"CD",  400},
    {"C",   100},
    {"XC",   90},
    {"L",    50},
    {"XL",   40},
    {"X",    10},
    {"IX",    9},
    {"V",     5},
    {"IV",    4},
    {"I",     1},
};

std::string number_as_roman(int iNumber) {
	// Strictly speaking, Roman digits can't display something
	// such as 4999 without using overlaid bars and so forth,
	// but for now this is a quick-and-dirty piece of code that'll
	// just keep using M's...
	//
	if (iNumber<=0) return "O";
	std::string result;
	for (unsigned int i=0; iNumber && i<sizeof(roman_digits)/sizeof(roman_digits[0]); i++) {
		while (roman_digits[i].decimal<=iNumber ) {
			result+=roman_digits[i].roman;
			iNumber-=roman_digits[i].decimal;
		}
	}
	return result;
}

// our work
void init_random_gen() {
	srand((unsigned int)time( NULL ));
}

#ifdef WIN32
std::string get_base_dir() {
	return std::string("./");
}
#else
#ifdef FREESIEGE_ISINSTALLED
std::string get_base_dir() {
	return std::string("/usr/share/freesiege");
}
#else
std::string get_base_dir() {
	std::stringstream link;
	link<<"/proc/"<<getpid()<<"/exe";
	char full_path[512];
	int len;
	if ((len=readlink(link.str().c_str(),full_path,sizeof(full_path)-1))!=-1) full_path[len]='\0';
	while (len>=0 && full_path[len]!='/') full_path[len--]='\0';
	return std::string(full_path);
}
#endif
#endif

