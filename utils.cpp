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

#include "plateformSDL.h"
#include "param.h"
#include <sstream>

#include <ctime>
#include <cstdlib>

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

TextureIds init_opengl(int width, int height,unsigned int n_texture) {
	//General init
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glClearColor(0,0,0,1);
	glClearDepth(1);// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);// The Type Of Depth Testing To Do
	glEnable(GL_BLEND);//Alpha blending
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);// Really Nice Perspective Calculations

	//Textures init
	TextureIds texture_ids=new TextureId[n_texture];
	glGenTextures(n_texture,texture_ids);

	//Set projection to orthogonal
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,width,height,0,1.0,-1.0);
	//glFrustum(0,width,height,0,0,10);

	//Go to model matrix mode
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	return texture_ids;
}

void fill_rect_opengl(float x,float y,float w,float h,float r,float g,float b,float alpha) {
	glBindTexture(GL_TEXTURE_2D,0);
	glPushMatrix();
	glTranslatef(x,y,0.0);
	glColor4f(r,g,b,alpha);
	glNormal3f(0.0,0.0,1.0);
	glBegin(GL_QUADS);
	glVertex3f(0,0,0);
	glVertex3f(w,0,0);
	glVertex3f(w,h,0);
	glVertex3f(0,h,0);
	glEnd();
	glPopMatrix();
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

SDL_Surface *load_surface(const std::string &filename) {
	SDL_Surface *load_surf=SDL_LoadBMP(filename.c_str());
	if (!load_surf) return NULL;

	//SDL_SetColorKey(load_surf,SDL_SRCCOLORKEY|SDL_RLEACCEL,SDL_MapRGB(load_surf->format,0xff,0x00,0xff));
	SDL_Surface *optimized_surf=SDL_DisplayFormatAlpha(load_surf);
	SDL_FreeSurface(load_surf);
	if (!optimized_surf) return NULL;
	
	SDL_SetColorKey(optimized_surf,SDL_SRCCOLORKEY|SDL_RLEACCEL,SDL_MapRGB(optimized_surf->format,0xff,0x00,0xff));
	SDL_SetAlpha(optimized_surf,0,0);	
	return optimized_surf;
}

float factor(float a) {
	return float(a+(1.0-a)*rand()/RAND_MAX);
}

void draw_fadein(const Sprite *sprite,float x,float y,int count,int end,int delay,float n_turn,float angle) {
	if (count>end-delay && count<=end) {
		float factor=float(count-end+delay)/(delay+1);
		sprite->draw(x,y,factor,angle+360*n_turn*(1-factor),factor);
	} else if (count>end) sprite->draw(x,y,1.0,angle);
}

void draw_fadeout(const Sprite *sprite,float x,float y,int count,int end,int delay,float n_turn,float angle) {
	if (count<=end-delay) sprite->draw(x,y,1.0,angle);
	else if (count<=end) {
		float factor=1-float(count-end+delay)/(delay+1);
		sprite->draw(x,y,factor,angle+360*n_turn*factor,factor);
	}
}
