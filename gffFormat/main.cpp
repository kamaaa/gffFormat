/*
 * main.cpp
 *
 * Created on: 14 sty 2015
 * Author: team15
 *
 */

#include <iostream>
#include <vector>
#include <stdint.h>
#include <bitset>
#include "class/GffFormat.h"
#include <gtkmm-3.0/gtkmm.h>
#include <gdk-pixbuf-2.0/gdk-pixbuf/gdk-pixbuf.h>
#include <cairomm-1.0/cairomm.h>
#include <cairomm-1.0/cairomm/context.h>
#include <glibmm-2.4/glibmm.h>
#include "Da.h"
#include "class/Stage.h"

using namespace std;

void button_clicked(){
	cout<<"Clicked"<<endl;
}

int main(int argc, char *argv[]){
	cout<<"New projekt"<<endl;

	/*GffFormat g;
	g.loadBitmap("images/Koala.bmp");
	vector<unsigned char> pixels = g.getPixels();*/

	Stage stage(argc, argv, "GFF Format", 600, 400);
	stage.addMainMenu();

	stage.addStage();

	/*Gtk::Window window;
	window.set_default_size(600, 400);
	window.set_title("GFF Format");
	window.set_position(Gtk::WIN_POS_CENTER);
	window.set_border_width(10);
	window.set_size_request(300,300);*/

    /* Pierwszy sposob
    Gtk::Box *vbox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0));
    window.add(*vbox);

    Gtk::MenuBar *menubar = Gtk::manage(new Gtk::MenuBar());
    vbox->pack_start(*menubar, Gtk::PACK_SHRINK, 0);

    Gtk::MenuItem *menuitem_file = Gtk::manage(new Gtk::MenuItem("_File", true));
    menubar->append(*menuitem_file);
    Gtk::Menu *filemenu = Gtk::manage(new Gtk::Menu());
    menuitem_file->set_submenu(*filemenu);

    Gtk::MenuItem *menuitem_quit = Gtk::manage(new Gtk::MenuItem("_Quit", true));
    filemenu->append(*menuitem_quit);

    menuitem_quit->signal_activate().connect(&button_clicked);

    vbox->show_all();*/

	/*Drugi sposob
	Gtk::Box *pbox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0));
	window.add(*pbox);

	Glib::RefPtr<Gtk::ActionGroup> action = Gtk::ActionGroup::create();
	Glib::RefPtr<Gtk::UIManager> uiManager = Gtk::UIManager::create();

	action->add(Gtk::Action::create("MenuFile","_File"));
	action->add(Gtk::Action::create("New", "_New"), &button_clicked);

	uiManager->insert_action_group(action);
	Glib::ustring ui_info =
	    "<ui>"
	    "  <menubar name='MenuBar'>"
	    "    <menu action='MenuFile'>"
	    "      <menuitem action='New'/>"
	    "    </menu>"
		"	</menubar>"
		"</ui>";
	uiManager->add_ui_from_string(ui_info);
	Gtk::Widget *pMenubar = uiManager->get_widget("/MenuBar");
	pbox->pack_start(*pMenubar, Gtk::PACK_SHRINK);

	pbox->show_all();
	*/
	/*Gtk::DrawingArea da;
	Cairo::RefPtr<Cairo::ImageSurface> imageSourface = Cairo::ImageSurface::create(Cairo::FORMAT_RGB24, 1024,  768);
	Cairo::RefPtr<Cairo::Context> crr = Cairo::Context::create(imageSourface);
	//Cairo::RefPtr<Cairo::Context> cr = da.get_window()->create_cairo_context();
	//Cairo::Context* cr;

	Glib::RefPtr<Gdk::Pixbuf> image = Gdk::Pixbuf::create_from_data(pixels.data(),Gdk::COLORSPACE_RGB, false, 8, 1024,768,1024*3);
	Gdk::Cairo::set_source_pixbuf(crr, image, 1024, 768);

	da.on_draw(crr);
	crr->paint();
	//da.draw(cr);
	//da.draw(cr);*/
	/*Da area;
	window.add(area);
	area.show();

	window.show_all();


	return app->run(window);*/
	return 0;
	//return 0;
}
