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
#include <vector>
#include <gtkmm-3.0/gtkmm/window.h>
#include <gtkmm-3.0/gtkmm/application.h>
#include <gtkmm-3.0/gtkmm/filefilter.h>
#include <gdk-pixbuf-2.0/gdk-pixbuf/gdk-pixbuf.h>

#include "GffFormat.h"
#include "BmpFormat.h"
#include "DisplayObject.h"

using namespace std;

class Stage {
private:
	string title;
	int stageWidth;
	int stageHeight;

	Glib::RefPtr<Gtk::Application> app;
	Gtk::Window *window;
	Gtk::Box *mainBox;

	Gtk::Box* mainMenuBox;
	Gtk::MenuBar* mainMenuBar;
	Gtk::Image* checkMarkIcon;
	Gtk::ImageMenuItem* colorSelected;
	vector<Gtk::MenuItem*> saveItems;

	DisplayObject* drawArea;
	GffFormat gff;
	BmpFormat bmp;
	string path;


public:
	Stage(int argc, char* argv[]){
		this->app = Gtk::Application::create(argc, argv,"org.gtkmm.examples.base");

		this->window = new Gtk::Window;
		this->title = "GFF Format";
		this->stageWidth = 600;
		this->stageHeight = 400;

		this->mainBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 0));

		this->drawArea = new DisplayObject;
		this->mainMenuBar = NULL;
		this->mainMenuBox = NULL;
		this->checkMarkIcon = NULL;
		this->colorSelected = NULL;

		this->saveItems.resize(3);
	}
	Stage(int argc, char* argv[], string title, int width, int height){
		this->app = Gtk::Application::create(argc, argv,"org.gtkmm.examples.base");

		this->window = new Gtk::Window;
		this->title = title;
		this->stageWidth = width;
		this->stageHeight = height;

		this->mainBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0));

		this->drawArea = new DisplayObject;
		this->mainMenuBar = NULL;
		this->mainMenuBox = NULL;
		this->checkMarkIcon = NULL;
		this->colorSelected = NULL;

		this->window->set_default_size(this->stageWidth, this->stageHeight);
		this->window->set_title(this->title);
		this->window->set_position(Gtk::WIN_POS_CENTER);
		this->window->set_icon_from_file("icon.png");

		this->saveItems.resize(3);
		this->mainBox->show_all();
		//this->mainBox->set_size_request(this->stageWidth, this->stageHeight);
		this->window->add(*mainBox);
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

		this->mainBox->add(*this->mainMenuBox);
		this->mainMenuBox->pack_start(*this->mainMenuBar, Gtk::PACK_SHRINK, 0);
		this->checkMarkIcon->set_halign(Gtk::ALIGN_START);

		this->addMenuFileItem();
		this->addMenuColorItem();
		this->addMenuHelpItem();

		this->mainMenuBox->show_all();
	}

	void resizeWindow(int width, int height){
		this->window->resize(width, height);
		this->stageWidth = width;
		this->stageHeight = height;
	}

private:
	void addMenuFileItem(){
		Gtk::Menu* fileSubMenu = this->addMenuCategory("Plik");

		Gtk::MenuItem* openFile = Gtk::manage(new Gtk::MenuItem("_Otwórz plik", true));
		Gtk::MenuItem* saveFile = Gtk::manage(new Gtk::MenuItem("_Zapisz", true));
		Gtk::MenuItem* saveFileAsBMP = Gtk::manage(new Gtk::MenuItem("_Zapisz jako BMP", true));
		Gtk::MenuItem* saveFileAsGFF = Gtk::manage(new Gtk::MenuItem("_Zapisz jako GFF", true));
		Gtk::MenuItem* closeProgram = Gtk::manage(new Gtk::MenuItem("_Zakończ", true));
		Gtk::SeparatorMenuItem* closeSeparator = Gtk::manage(new Gtk::SeparatorMenuItem());

		openFile->signal_activate().connect(sigc::mem_fun(*this, &Stage::openFileEvent));
		saveFile->signal_activate().connect(sigc::mem_fun(*this, &Stage::saveFileEvent));
		saveFileAsBMP->signal_activate().connect(sigc::bind<string>(sigc::mem_fun(*this, &Stage::saveFileAsEvent),"bmp"));
		saveFileAsGFF->signal_activate().connect(sigc::bind<string>(sigc::mem_fun(*this, &Stage::saveFileAsEvent),"gff"));
		closeProgram->signal_activate().connect(sigc::ptr_fun(&Gtk::Main::quit));

		this->saveItems[0] = saveFile;
		this->saveItems[1] = saveFileAsBMP;
		this->saveItems[2] = saveFileAsGFF;

		this->enableSaveButtons(false);

		fileSubMenu->append(*openFile);
		fileSubMenu->append(*saveFile);
		fileSubMenu->append(*saveFileAsBMP);
		fileSubMenu->append(*saveFileAsGFF);
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
		Gtk::FileChooserDialog openFileDialog("Otwórz plik",Gtk::FILE_CHOOSER_ACTION_OPEN);
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
		    	string type;

		        cout << "Open clicked." << endl;
		        this->path = openFileDialog.get_filename();
		        cout << "File selected: " <<  this->path << endl;

		        window->set_title(this->title+" - "+this->path);
		        this->enableSaveButtons(true);
		        type = this->path.substr(this->path.length()-3,3);

		        if(type=="bmp"){
		        	this->bmpFileOpened();
		        }else{
		        	this->gffFileOpened();
		        }

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
	void saveFileEvent(){

	}
	void saveFileAsEvent(string type){
		Gtk::FileChooserDialog saveFileDialog("Zapisz plik jako "+type,Gtk::FILE_CHOOSER_ACTION_SAVE);
		Glib::RefPtr<Gtk::FileFilter> imgFilter = Gtk::FileFilter::create();

		saveFileDialog.set_transient_for(*this->window);

		saveFileDialog.add_button("_Anuluj", Gtk::RESPONSE_CANCEL);
		saveFileDialog.add_button("_Zapisz", Gtk::RESPONSE_OK);

		imgFilter->set_name("Pliki graficzne (."+type+")");
		imgFilter->add_pattern("*."+type);

		saveFileDialog.add_filter(imgFilter);
		int result = saveFileDialog.run();

				//Handle the response:
		switch(result){
			case(Gtk::RESPONSE_OK):{
				// The user selected a file

				cout << "Save clicked." << endl;
				string filename = saveFileDialog.get_filename();
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
	void enableSaveButtons(bool state){
		for(int i=0; i<(int)this->saveItems.size(); i++){
			this->saveItems[i]->set_sensitive(state);
		}
	}

	bool bmpFileOpened(){
		gff.loadBitmap(this->path.data());
		drawArea->setPixels(gff.getPixels());
		drawArea->setResolution(gff.getWidth(),gff.getHeight());

		drawArea->show();
		this->mainBox->add(*drawArea);
		return false;
	}
	bool gffFileOpened(){

		return false;
	}
};

#endif /* CLASS_STAGE_H_ */
