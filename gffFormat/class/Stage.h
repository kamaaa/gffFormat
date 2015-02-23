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
#include <gtkmm-3.0/gtkmm/main.h>
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
	int screenWidth;
	int screenHeight;

	Glib::RefPtr<Gtk::Application> app;
	Gtk::Window *window;
	Gtk::Box *mainBox;

	Gtk::Box* mainMenuBox;
	Gtk::MenuBar* mainMenuBar;
	Gtk::Image* checkMarkIcon;
	vector<Gtk::MenuItem*> saveItems;
	vector<Gtk::ImageMenuItem*> colorItems;
	unsigned short colorSelected;

	DisplayObject* drawArea;
	GffFormat gff;
	BmpFormat bmp;
	LzCompressor lz;
	string path;
	string format;


public:
	Stage(int argc, char* argv[]){
		this->app = Gtk::Application::create(argc, argv,"org.gtkmm.examples.base");

		this->window = new Gtk::Window;
		this->title = "GFF Format";
		this->stageWidth = 600;
		this->stageHeight = 400;
		this->screenWidth = Gdk::screen_width();
		this->screenHeight = Gdk::screen_height();

		this->mainBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 0));

		this->drawArea = new DisplayObject;
		this->mainMenuBar = NULL;
		this->mainMenuBox = NULL;
		this->checkMarkIcon = NULL;
		this->colorSelected = 0;

		this->saveItems.resize(2);
		this->colorItems.resize(5);
		this->mainBox->show_all();
		this->mainBox->add(*drawArea);
	}
	Stage(int argc, char* argv[], string title, int width, int height){
		this->app = Gtk::Application::create(argc, argv,"org.gtkmm.examples.base");

		this->window = new Gtk::Window;
		this->title = title;
		this->stageWidth = width;
		this->stageHeight = height;
		this->screenWidth = Gdk::screen_width();
		this->screenHeight = Gdk::screen_height();

		this->mainBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0));

		this->drawArea = new DisplayObject(width, height);
		this->mainMenuBar = NULL;
		this->mainMenuBox = NULL;
		this->checkMarkIcon = NULL;
		this->colorSelected = 0;

		this->window->set_default_size(this->stageWidth, this->stageHeight);
		this->window->set_title(this->title);
		this->window->set_position(Gtk::WIN_POS_CENTER);
		this->window->set_icon_from_file("icon.png");

		this->saveItems.resize(2);
		this->colorItems.resize(5);
		this->mainBox->add(*this->drawArea);
		this->mainBox->show_all();
		this->window->add(*mainBox);
	}

	void createWindow(){
		this->window->set_default_size(this->stageWidth, this->stageHeight);
		this->window->set_title(this->title);
		this->window->set_position(Gtk::WIN_POS_CENTER);
		this->window->set_icon_from_file("icon.png");

		this->window->add(*this->mainBox);
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
		this->mainBox->reorder_child(*this->mainMenuBox, 0);
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
		//Gtk::MenuItem* saveFile = Gtk::manage(new Gtk::MenuItem("_Zapisz", true));
		Gtk::MenuItem* saveFileAsBMP = Gtk::manage(new Gtk::MenuItem("_Zapisz jako BMP", true));
		Gtk::MenuItem* saveFileAsGFF = Gtk::manage(new Gtk::MenuItem("_Zapisz jako GFF", true));
		Gtk::MenuItem* closeProgram = Gtk::manage(new Gtk::MenuItem("_Zakończ", true));
		Gtk::SeparatorMenuItem* closeSeparator = Gtk::manage(new Gtk::SeparatorMenuItem());

		openFile->signal_activate().connect(sigc::mem_fun(*this, &Stage::openFileEvent));
		//saveFile->signal_activate().connect(sigc::mem_fun(*this, &Stage::saveFileEvent));
		saveFileAsBMP->signal_activate().connect(sigc::bind<string>(sigc::mem_fun(*this, &Stage::saveFileAsEvent),"bmp"));
		saveFileAsGFF->signal_activate().connect(sigc::bind<string>(sigc::mem_fun(*this, &Stage::saveFileAsEvent),"gff"));
		closeProgram->signal_activate().connect(sigc::ptr_fun(&Gtk::Main::quit));

		//this->saveItems[0] = saveFile;
		this->saveItems[0] = saveFileAsBMP;
		this->saveItems[1] = saveFileAsGFF;

		this->enableSaveButtons(false);

		fileSubMenu->append(*openFile);
		//fileSubMenu->append(*saveFile);
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

		this->colorItems[0] = rgbColor;
		this->colorItems[1] = bgrColor;
		this->colorItems[2] = hslColor;
		this->colorItems[3] = hsvColor;
		this->colorItems[4] = yuvColor;

		this->colorSelected = ColorSettings::RGB;
		this->setActiveColorSpace(this->colorSelected);

		rgbColor->signal_activate().connect(sigc::bind <int>(sigc::mem_fun(*this, &Stage::setActiveColorSpace), ColorSettings::RGB), false);
		bgrColor->signal_activate().connect(sigc::bind <int>(sigc::mem_fun(*this, &Stage::setActiveColorSpace), ColorSettings::BGR), false);
		hslColor->signal_activate().connect(sigc::bind <int>(sigc::mem_fun(*this, &Stage::setActiveColorSpace), ColorSettings::HSL), false);
		hsvColor->signal_activate().connect(sigc::bind <int>(sigc::mem_fun(*this, &Stage::setActiveColorSpace), ColorSettings::HSV), false);
		yuvColor->signal_activate().connect(sigc::bind <int>(sigc::mem_fun(*this, &Stage::setActiveColorSpace), ColorSettings::YUV), false);

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

	void setActiveColorSpace(unsigned short colorID){
		Gtk::ImageMenuItem* currentItem = this->colorItems[this->colorSelected];
		currentItem->set_always_show_image(false);

		this->colorItems[colorID]->set_always_show_image(true);
		this->colorSelected = colorID;

		switch(colorID){
		case(ColorSettings::RGB):{
			cout<<"RGB"<<endl;
			break;
			}
		case(ColorSettings::BGR):{
			cout<<"BGR"<<endl;
			break;
			}
		case(ColorSettings::HSL):{
			cout<<"HSL"<<endl;
			break;
			}
		case(ColorSettings::HSV):{
			cout<<"HSV"<<endl;
			break;
			}
		case(ColorSettings::YUV):{
			cout<<"YUV"<<endl;
			break;
			}
		}

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

		        cout << "Open clicked." << endl;
		        this->path = openFileDialog.get_filename();
		        cout << "File selected: " <<  this->path << endl;

		        window->set_title(this->title+" - "+this->path);
		        this->enableSaveButtons(true);
		        this->format = this->path.substr(this->path.length()-3,3);

		        if(this->format=="bmp"){
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
		cout<<"save..."<<endl;
		if(this->path == ""){
			cout<<"no path"<<endl;
			return;
		}
		if(this->format == "gff"){
			gff.saveGff(this->path.data());
		}else if(this->format == "bmp"){
			bmp.saveBmp(this->path.data());
		}
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

		switch(result){
			case(Gtk::RESPONSE_OK):{
				// The user selected a file
				cout << "Save clicked." << endl;
				this->path = saveFileDialog.get_filename();
				this->format = this->path.substr(this->path.length()-3,3);

				if(this->format != type){
					this->format = type;
					this->path += "." + type;
				}
				cout << "File selected: " <<  this->path << endl;

				this->saveFileEvent();
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
		int imageWidth = gff.getWidth();
		int imageHeight = gff.getHeight();

		this->setActiveColorSpace(ColorSettings::BGR);
		this->saveItems[0]->set_sensitive(false);

		if(imageHeight > this->screenHeight){
			this->scaleWindow(false, imageWidth, imageHeight);
		}else if(imageHeight > this->stageHeight){
			this->scaleWindow(true, imageWidth, imageHeight);
		}else{
			drawArea->setWindowResulution(this->stageWidth, this->stageHeight);
			drawArea->scaleImage(false);
		}

		drawArea->setPixels(gff.getRawPixels(), "bmp", ColorSettings::BGR);
		drawArea->setImageResolution(imageWidth, imageHeight);

		drawArea->show();

		return true;
	}
	bool gffFileOpened(){
		bmp.loadGff(this->path.data());
		int imageWidth = bmp.getWidth();
		int imageHeight = bmp.getHeight();

		string colorspace = bmp.getColorspace();
		this->saveItems[1]->set_sensitive(false);

		if(colorspace == "HSL"){
			this->setActiveColorSpace(ColorSettings::HSL);
		}else if(colorspace == "HSV"){
			this->setActiveColorSpace(ColorSettings::HSV);
		}else if(colorspace == "YUV"){
			this->setActiveColorSpace(ColorSettings::YUV);
		}else{
			this->setActiveColorSpace(ColorSettings::RGB);
		}

		if(imageHeight > this->screenHeight){
			this->scaleWindow(false, imageWidth, imageHeight);
		}else if(imageHeight > this->stageHeight){
			this->scaleWindow(true, imageWidth, imageHeight);
		}else{
			drawArea->setWindowResulution(this->stageWidth, this->stageHeight);
			drawArea->scaleImage(false);
		}

		drawArea->setPixels(bmp.getRawPixels(), "gff", this->colorSelected);
		drawArea->setImageResolution(imageWidth, imageHeight);

		drawArea->show();

		return false;
	}
	void scaleWindow(bool adjust, int imageWidth, int imageHeight){
		double ratio = (double)imageWidth / imageHeight;
		int newWindowHeight = 0;
		int newWindowWidth = 0;

		if(!adjust){
			newWindowHeight = this->screenHeight - 100;
		}else{
			newWindowHeight = imageHeight;
		}

		newWindowWidth = newWindowHeight * ratio;
		this->resizeWindow(newWindowWidth, newWindowHeight);
		drawArea->scaleImage(true);
		drawArea->setWindowResulution(this->stageWidth, this->stageHeight);
	}
};

#endif /* CLASS_STAGE_H_ */
