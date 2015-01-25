/*
 * DisplayObject.h
 *
 *  Created on: 25 sty 2015
 *      Author: kama
 */

#ifndef CLASS_DISPLAYOBJECT_H_
#define CLASS_DISPLAYOBJECT_H_

#include <gtkmm-3.0/gtkmm/drawingarea.h>
#include <gdk-pixbuf-2.0/gdk-pixbuf/gdk-pixbuf.h>
#include <vector>

using namespace std;

class DisplayObject: public Gtk::DrawingArea {
protected:
	Glib::RefPtr<Gdk::Pixbuf> image;
	vector<unsigned char> pixels;

public:
	DisplayObject(){

	}
	void setResolution(int width, int height){
		this->set_size_request(width, height);
	}
	void setPixels(vector<unsigned char> arrayPixel){
		this->pixels = arrayPixel;
	}

protected:
	virtual bool on_draw(const Cairo::RefPtr<Cairo::Context> &cr){
		if(this->pixels.size() == 0){
			return false;
		}
		image = Gdk::Pixbuf::create_from_data(this->pixels.data(),Gdk::COLORSPACE_RGB, false, 8, 1024,768,1024*3);
		Gdk::Cairo::set_source_pixbuf(cr, image, 0, 0);

		cr->paint();

		return true;
	}
};

#endif /* CLASS_DISPLAYOBJECT_H_ */
