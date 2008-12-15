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
#include "sprite.h"

#include <iostream>

//helper function
int nearest_power(int number) {
	int i;
	for (i=1;i<number;i=i<<1);
	return i;
}

Sprite::Sprite(SDL_Surface *surf,TextureId id) {
	//FIXME SDL_ttf seems to put a bad width in the pixel format... using pitch instead
	int real_w =surf->pitch/surf->format->BytesPerPixel;
	//if (real_width!=surf->w) std::cerr<<"warning bad surface width contition pitch:"<<real_width<<" width:"<<surf->w<<std::endl;

	//compute factors
	int n_w=nearest_power(real_w); //OpenGL wants specific sizes for textures
	int n_h=nearest_power(surf->h);

	glBindTexture(GL_TEXTURE_2D,id);
	unsigned char *texture=new unsigned char[n_w*n_h*4];
	unsigned char *iter=texture;
	unsigned char *end=texture+n_w*n_h*4;
	Uint32 transparent=0;
	if (surf->flags & SDL_SRCCOLORKEY) transparent=SDL_MapRGB(surf->format,0xff,0x00,0xff);

	Uint8 *char_pixel=static_cast<Uint8*>(surf->pixels);
	Uint32 *int_pixel=static_cast<Uint32*>(surf->pixels);

	switch (surf->format->BitsPerPixel) {
	case 32: //RGBA image type
		while (iter!=end) {
			if ((iter-texture)/4%n_w >= real_w || (iter-texture)/4/n_w >= surf->h) {
				iter[3]=0; //Outside the pixmap, the texture is fully transparent
				iter[2]=0;
				iter[1]=0;
				iter[0]=0;
			} else {
				if (*int_pixel==transparent) iter[3]=0x00;  //alpha
				else iter[3]=0xff;
				iter[2]=(*int_pixel & surf->format->Bmask)>>surf->format->Bshift;
				iter[1]=(*int_pixel & surf->format->Gmask)>>surf->format->Gshift;
				iter[0]=(*int_pixel & surf->format->Rmask)>>surf->format->Rshift;
				int_pixel++;
			}
			iter+=4;
		}
		break;
	case 8: //indexed image
		while (iter!=end) {
			if ((iter-texture)/4%n_w >= real_w || (iter-texture)/4/n_w >= surf->h) {
				iter[3]=0; //Outside the pixmap, the texture is fully transparent
				iter[2]=0;
				iter[1]=0;
				iter[0]=0;
			} else {
				SDL_Color color=surf->format->palette->colors[*char_pixel];
				if (SDL_MapRGB(surf->format,color.r,color.g,color.b)==transparent) iter[3]=0x00;  //alpha
				else iter[3]=0xff;
				iter[2]=color.b;
				iter[1]=color.g;
				iter[0]=color.r;
				char_pixel++;
			}
			iter+=4;
		}
		break;
	default:
		std::cerr<<"unsupported depth "<<int(surf->format->BitsPerPixel)<<std::endl;
		break;
	}
	glTexImage2D(GL_TEXTURE_2D,0,4,n_w,n_h,0,GL_RGBA,GL_UNSIGNED_BYTE,texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	delete [] texture;
	
	this->w=real_w;
	this->h=surf->h;
	this->factor_w=float(real_w)/n_w;
	this->factor_h=float(surf->h)/n_h;
	this->texture_id=id;
}

void Sprite::draw(float x, float y, float alpha, float angle, float factor) const {
	glBindTexture(GL_TEXTURE_2D,texture_id);
	glColor4f(1.0,1.0,1.0,alpha);
	glPushMatrix();
	glTranslatef(x+w/2,y+h/2,0.0);
	glRotatef(angle,0.0,0.0,1.0);
	glNormal3f(0.0,0.0,1.0);
	glBegin(GL_QUADS);
	glTexCoord2f(factor_w,factor_h); glVertex3f(factor*w/2,factor*h/2,0);
	glTexCoord2f(factor_w,0.0); glVertex3f(factor*w/2,-factor*h/2,0);
	glTexCoord2f(0.0,0.0); glVertex3f(-factor*w/2,-factor*h/2,0);
	glTexCoord2f(0.0,factor_h); glVertex3f(-factor*w/2,factor*h/2,0);
	glEnd();
	glPopMatrix();
}

void Sprite::draw_flip_h(float x, float y, float alpha, float angle, float factor) const {
	glBindTexture(GL_TEXTURE_2D,texture_id);
	glColor4f(1.0,1.0,1.0,alpha);
	glPushMatrix();
	glTranslatef(x+w/2,y+h/2,0.0);
	glRotatef(angle,0.0,0.0,1.0);
	glNormal3f(0.0,0.0,1.0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0,factor_h); glVertex3f(factor*w/2,factor*h/2,0);
	glTexCoord2f(0.0,0.0); glVertex3f(factor*w/2,-factor*h/2,0);
	glTexCoord2f(factor_w,0.0); glVertex3f(-factor*w/2,-factor*h/2,0);
	glTexCoord2f(factor_w,factor_h); glVertex3f(-factor*w/2,factor*h/2,0);
	glEnd();
	glPopMatrix();
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
