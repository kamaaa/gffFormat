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
#include "LzCompressor.h"

using namespace std;

class DisplayObject: public Gtk::DrawingArea {
protected:
	Glib::RefPtr<Gdk::Pixbuf> image;
	int imageWidth;
	int imageHeight;
	int stageWidth;
	int stageHeight;
	bool scale;

	string imageFormat;
	vector<uint8_t> pixels;

public:
	DisplayObject(){
		this->imageWidth=0;
		this->imageHeight=0;
		this->stageWidth=0;
		this->stageHeight=0;
		this->imageFormat = "";
		this->scale = true;
	}
	DisplayObject(int stageWidth, int stageHeight){
		this->stageWidth=stageWidth;
		this->stageHeight=stageHeight;
		this->imageWidth=0;
		this->imageHeight=0;
		this->imageFormat = "";
		this->scale = true;
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

	void scaleImage(bool scaleMode){
		this->scale = scaleMode;
	}

	void setPixels(vector<uint8_t> arrayPixel, string type, unsigned short colorSpace){
		this->pixels = arrayPixel;
		cout<<this->pixels.size()<<endl;
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
		double x=0;
		double y=0;

		if(this->pixels.size() == 0 || this->imageWidth == 0 || this->imageHeight == 0){
			return false;
		}

		this->image = Gdk::Pixbuf::create_from_data(this->pixels.data(),Gdk::COLORSPACE_RGB, false, 8, this->imageWidth,this->imageHeight,this->imageWidth*3);
		this->image = this->image->flip(false);

		if(this->scale){
			cr->scale((double)this->stageWidth/this->image->get_width(),(double)this->stageHeight/this->image->get_height());

			Gdk::Cairo::set_source_pixbuf(cr, this->image, 0,0);
			cr->rectangle(0,0,this->image->get_width(),this->image->get_height());
		}else{
			x = (double)this->stageWidth/2 - this->image->get_width()/2;
			y = (double)this->stageHeight/2 - this->image->get_height()/2;

			Gdk::Cairo::set_source_pixbuf(cr, this->image, x,y);
			cr->rectangle(x,y,this->image->get_width(),this->image->get_height());
		}

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
