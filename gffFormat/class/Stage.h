/*
 * Stage.h
 *
 *  Created on: 19 sty 2015
 *      Author: kama
 */

#ifndef CLASS_STAGE_H_
#define CLASS_STAGE_H_

#include <iostream>
#include <string>
#include <gtkmm-3.0/gtkmm/window.h>
#include <gtkmm-3.0/gtkmm/application.h>
#include <gdk-pixbuf-2.0/gdk-pixbuf/gdk-pixbuf.h>

using namespace std;

class Stage {
private:
	string title;
	int stageWidth;
	int stageHeight;

	Glib::RefPtr<Gtk::Application> app;
	Gtk::Window *window;

	Gtk::Box* mainMenuBox;
	Gtk::MenuBar* mainMenuBar;
	Gtk::Image* checkMarkIcon;
	Gtk::MenuItem* currentItem;


public:
	Stage(int argc, char* argv[]){
		this->app = Gtk::Application::create(argc, argv,"org.gtkmm.examples.base");

		this->window = new Gtk::Window;
		this->title = "GFF Format";
		this->stageWidth = 600;
		this->stageHeight = 400;

	}
	Stage(int argc, char* argv[], string title, int width, int height){
		this->app = Gtk::Application::create(argc, argv,"org.gtkmm.examples.base");

		this->window = new Gtk::Window;
		this->title = title;
		this->stageWidth = width;
		this->stageHeight = height;

		this->window->set_default_size(this->stageWidth, this->stageHeight);
		this->window->set_title(this->title);
		this->window->set_position(Gtk::WIN_POS_CENTER);
	}

	void createWindow(){
		this->window->set_default_size(this->stageWidth, this->stageHeight);
		this->window->set_title(this->title);
		this->window->set_position(Gtk::WIN_POS_CENTER);
	}

	int addStage(){
		this->window->show_all();
		return this->app->run(*this->window);
	}

	void addMainMenu(){
		this->mainMenuBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0));
		this->checkMarkIcon = new Gtk::Image("assets/checkmark1.png");
		this->window->add(*this->mainMenuBox);

		this->mainMenuBar = Gtk::manage(new Gtk::MenuBar());
		this->mainMenuBox->pack_start(*this->mainMenuBar, Gtk::PACK_SHRINK, 0);
		this->checkMarkIcon->set_halign(Gtk::ALIGN_START);

		this->addMenuFileItem();
		this->addMenuColorItem();
		this->addMenuHelpItem();

		this->mainMenuBox->show_all();
	}

private:
	void addMenuFileItem(){
		Gtk::Menu* fileSubMenu = this->addMenuCategory("Plik");

		Gtk::MenuItem* openFile = Gtk::manage(new Gtk::MenuItem("_Otwórz", true));
		Gtk::MenuItem* saveFile = Gtk::manage(new Gtk::MenuItem("_Zapisz", true));
		Gtk::MenuItem* closeProgram = Gtk::manage(new Gtk::MenuItem("_Zakończ", true));
		Gtk::SeparatorMenuItem* closeSeparator = Gtk::manage(new Gtk::SeparatorMenuItem());

		fileSubMenu->append(*openFile);
		fileSubMenu->append(*saveFile);
		fileSubMenu->append(*closeSeparator);
		fileSubMenu->append(*closeProgram);
	}
	void addMenuColorItem(){
		Gtk::Menu* fileSubMenu = this->addMenuCategory("Paleta kolorów");


		Gtk::ImageMenuItem* rgbColor = Gtk::manage(new Gtk::ImageMenuItem("_RGB", true));
		Gtk::ImageMenuItem* bgrColor = Gtk::manage(new Gtk::ImageMenuItem("_BGR", true));
		Gtk::ImageMenuItem* hslColor = Gtk::manage(new Gtk::ImageMenuItem("_HSL", true));
		Gtk::ImageMenuItem* hsvColor = Gtk::manage(new Gtk::ImageMenuItem("_HSV", true));
		Gtk::ImageMenuItem* yuvColor = Gtk::manage(new Gtk::ImageMenuItem("_YUV", true));

		rgbColor->set_image(*this->checkMarkIcon);
		/*hslColor->set_image(*this->checkMarkIcon);
		hsvColor->set_image(*this->checkMarkIcon);
		yuvColor->set_image(*this->checkMarkIcon);*/

		this->setActiveMenuItem(rgbColor);

		fileSubMenu->append(*rgbColor);
		fileSubMenu->append(*bgrColor);
		fileSubMenu->append(*hslColor);
		fileSubMenu->append(*hsvColor);
		fileSubMenu->append(*yuvColor);
	}
	void addMenuHelpItem(){
		Gtk::Menu* fileSubMenu = this->addMenuCategory("Pomoc");

		Gtk::MenuItem* aboutProgram = Gtk::manage(new Gtk::MenuItem("_O programie GFF Format", true));

		fileSubMenu->append(*aboutProgram);
	}
	Gtk::Menu* addMenuCategory(string name){
		Gtk::Menu* subMenu = Gtk::manage(new Gtk::Menu());
		Gtk::MenuItem* subMenuItem = Gtk::manage(new Gtk::MenuItem("_"+name, true));

		subMenuItem->set_submenu(*subMenu);

		this->mainMenuBar->append(*subMenuItem);

		return subMenu;
	}

	void setActiveMenuItem(Gtk::ImageMenuItem* menuItem){
		menuItem->remove();
		menuItem->set_always_show_image(true);
	}

};

#endif /* CLASS_STAGE_H_ */
