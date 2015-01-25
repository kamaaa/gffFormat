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
#include <gtkmm-3.0/gtkmm/filefilter.h>
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
	Gtk::ImageMenuItem* colorSelected;


public:
	Stage(int argc, char* argv[]){
		this->app = Gtk::Application::create(argc, argv,"org.gtkmm.examples.base");

		this->window = new Gtk::Window;
		this->title = "GFF Format";
		this->stageWidth = 600;
		this->stageHeight = 400;
		this->mainMenuBar = NULL;
		this->mainMenuBox = NULL;
		this->checkMarkIcon = NULL;
		this->colorSelected = NULL;
	}
	Stage(int argc, char* argv[], string title, int width, int height){
		this->app = Gtk::Application::create(argc, argv,"org.gtkmm.examples.base");

		this->window = new Gtk::Window;
		this->title = title;
		this->stageWidth = width;
		this->stageHeight = height;
		this->mainMenuBar = NULL;
		this->mainMenuBox = NULL;
		this->checkMarkIcon = NULL;
		this->colorSelected = NULL;

		this->window->set_default_size(this->stageWidth, this->stageHeight);
		this->window->set_title(this->title);
		this->window->set_position(Gtk::WIN_POS_CENTER);
		this->window->set_icon_from_file("icon.png");
	}

	void createWindow(){
		this->window->set_default_size(this->stageWidth, this->stageHeight);
		this->window->set_title(this->title);
		this->window->set_position(Gtk::WIN_POS_CENTER);
		this->window->set_icon_from_file("icon.png");
	}

	int addStage(){
		this->window->show_all();
		return this->app->run(*this->window);
	}

	void addMainMenu(){
		this->mainMenuBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0));
		this->checkMarkIcon = new Gtk::Image("assets/checkmark.png");
		this->mainMenuBar = Gtk::manage(new Gtk::MenuBar());

		this->window->add(*this->mainMenuBox);
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

		openFile->signal_activate().connect(sigc::mem_fun(*this, &Stage::openFileEvent));
		closeProgram->signal_activate().connect(sigc::ptr_fun(&Gtk::Main::quit));

		fileSubMenu->append(*openFile);
		fileSubMenu->append(*saveFile);
		fileSubMenu->append(*closeSeparator);
		fileSubMenu->append(*closeProgram);
	}
	void addMenuColorItem(){
		Gtk::Menu* fileSubMenu = this->addMenuCategory("Paleta kolorów");

		Gtk::Image* rgbCheck = new Gtk::Image(this->checkMarkIcon->get_pixbuf());
		Gtk::Image* bgrCheck = new Gtk::Image(this->checkMarkIcon->get_pixbuf());
		Gtk::Image* hslCheck = new Gtk::Image(this->checkMarkIcon->get_pixbuf());
		Gtk::Image* hsvCheck = new Gtk::Image(this->checkMarkIcon->get_pixbuf());
		Gtk::Image* yuvCheck = new Gtk::Image(this->checkMarkIcon->get_pixbuf());

		Gtk::ImageMenuItem* rgbColor = Gtk::manage(new Gtk::ImageMenuItem(*rgbCheck, "_RGB", true));
		Gtk::ImageMenuItem* bgrColor = Gtk::manage(new Gtk::ImageMenuItem(*bgrCheck, "_BGR", true));
		Gtk::ImageMenuItem* hslColor = Gtk::manage(new Gtk::ImageMenuItem(*hslCheck, "_HSL", true));
		Gtk::ImageMenuItem* hsvColor = Gtk::manage(new Gtk::ImageMenuItem(*hsvCheck, "_HSV", true));
		Gtk::ImageMenuItem* yuvColor = Gtk::manage(new Gtk::ImageMenuItem(*yuvCheck, "_YUV", true));

		this->colorSelected = rgbColor;
		this->colorSelected->set_always_show_image(true);

		rgbColor->set_label("RGB");
		bgrColor->set_label("BGR");
		hslColor->set_label("HSL");
		hsvColor->set_label("HSV");
		yuvColor->set_label("YUV");

		rgbColor->signal_activate().connect(sigc::bind <Gtk::ImageMenuItem*>(sigc::mem_fun(*this, &Stage::setActiveMenuItem), rgbColor), false);
		bgrColor->signal_activate().connect(sigc::bind <Gtk::ImageMenuItem*>(sigc::mem_fun(*this, &Stage::setActiveMenuItem), bgrColor), false);
		hslColor->signal_activate().connect(sigc::bind <Gtk::ImageMenuItem*>(sigc::mem_fun(*this, &Stage::setActiveMenuItem), hslColor), false);
		hsvColor->signal_activate().connect(sigc::bind <Gtk::ImageMenuItem*>(sigc::mem_fun(*this, &Stage::setActiveMenuItem), hsvColor), false);
		yuvColor->signal_activate().connect(sigc::bind <Gtk::ImageMenuItem*>(sigc::mem_fun(*this, &Stage::setActiveMenuItem), yuvColor), false);

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

	void setActiveMenuItem(Gtk::ImageMenuItem* item){
		this->colorSelected->set_always_show_image(false);
		item->set_always_show_image(true);
		this->colorSelected = item;

		cout<<(string)item->get_label();
	}
	void openFileEvent(){
		Gtk::FileChooserDialog openFileDialog("Proszę wybrać plik",Gtk::FILE_CHOOSER_ACTION_OPEN);
		Glib::RefPtr<Gtk::FileFilter> imageFilter = Gtk::FileFilter::create();

		openFileDialog.set_transient_for(*this->window);

		openFileDialog.add_button("_Anuluj", Gtk::RESPONSE_CANCEL);
		openFileDialog.add_button("_Otwórz", Gtk::RESPONSE_OK);

		imageFilter->set_name("Pliki graficzne (.bmp, .gff)");
		imageFilter->add_pattern("*.bmp");
		imageFilter->add_pattern("*.gff");

		openFileDialog.add_filter(imageFilter);
		int result = openFileDialog.run();

		//Handle the response:
		switch(result){
		    case(Gtk::RESPONSE_OK):{
		        // The user selected a file
		        cout << "Open clicked." << endl;
		        string filename = openFileDialog.get_filename();
		        cout << "File selected: " <<  filename << endl;
		        break;
		    }
		    case(Gtk::RESPONSE_CANCEL):
		        // The user clicked cancel
		        cout << "Cancel clicked." << endl;
		        break;

		    default:
		        // The user closed the dialog box
		        cout << "Unexpected button clicked." << endl;
		        break;

		}
	}
};

#endif /* CLASS_STAGE_H_ */
