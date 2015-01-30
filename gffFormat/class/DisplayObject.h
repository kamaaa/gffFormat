/*
 * DisplayObject.h
 *
 *  Created on: 25 sty 2015
 *  Author: kama
 */

#ifndef CLASS_DISPLAYOBJECT_H_
#define CLASS_DISPLAYOBJECT_H_

#include <gtkmm-3.0/gtkmm/drawingarea.h>
#include <gdk-pixbuf-2.0/gdk-pixbuf/gdk-pixbuf.h>
#include <gdkmm/color.h>
#include <vector>
#include <bitset>
#include <iostream>
#include <string>
#include "ColorSettings.h"

using namespace std;

class DisplayObject: public Gtk::DrawingArea {
protected:
	Glib::RefPtr<Gdk::Pixbuf> image;
	int imageWidth;
	int imageHeight;
	int stageWidth;
	int stageHeight;

	string imageFormat;
	vector<unsigned char> pixels;

public:
	DisplayObject(){
		this->imageWidth=0;
		this->imageHeight=0;
		this->stageWidth=0;
		this->stageHeight=0;
		this->imageFormat = "";
	}
	DisplayObject(int stageWidth, int stageHeight){
		this->stageWidth=stageWidth;
		this->stageHeight=stageHeight;
		this->imageWidth=0;
		this->imageHeight=0;
		this->imageFormat = "";

	}
	void setImageResolution(int width, int height){
		this->imageWidth = width;
		this->imageHeight = height;
	}
	void setWindowResulution(int width, int height){
		this->stageWidth = width;
		this->stageHeight = height;
		this->set_size_request(width, height);
	}
	void setPixels(vector<unsigned char> arrayPixel, string type, unsigned short colorSpace){
		this->pixels = arrayPixel;
		this->imageFormat = type;

		switch(colorSpace){
		case(ColorSettings::BGR):
			this->pixels = ColorSettings::swapRedBlue(this->pixels);
			break;
		case(ColorSettings::HSL):
			break;
		case(ColorSettings::HSV):
			break;
		case(ColorSettings::YUV):
			break;
		}
	}

protected:
	virtual bool on_draw(const Cairo::RefPtr<Cairo::Context> &cr){

		if(this->pixels.size() == 0 || this->imageWidth == 0 || this->imageHeight == 0){
			return false;
		}

		image = Gdk::Pixbuf::create_from_data(this->pixels.data(),Gdk::COLORSPACE_RGB, false, 8, this->imageWidth,this->imageHeight,this->imageWidth*3);
		if(this->imageFormat == "bmp"){
			image = image->flip(false);
		}

		cr->scale((double)this->stageWidth/image->get_width(),(double)this->stageHeight/image->get_height());
		Gdk::Cairo::set_source_pixbuf(cr, image, 0,0);
		cr->rectangle(0,0,image->get_width(),image->get_height());
		cr->fill();

		return true;
	}
	void drawPixel(const Cairo::RefPtr<Cairo::Context> &cr, int x, int y, int r, int g, int b){
		cr->rectangle(x,y-1,1,1);
		cr->set_source_rgb((double)r/255,(double)g/255,(double)b/255);
		cr->fill();
	}
};

#endif /* CLASS_DISPLAYOBJECT_H_ */
