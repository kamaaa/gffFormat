/*
 * main.cpp
 *
 * Created on: 14 sty 2015
 * Author: team15
 *
 */

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <vector>
#include <stdint.h>
#include <bitset>
#include <GL/freeglut.h>
#include "class/GraphicsInterface.h"
#include "class/GffFormat.h"


using namespace std;

int main(int argc, char** argv){
	cout<<"New projekt"<<endl;

	GffFormat g;
	g.loadBitmap("images/Koala.bmp");
	vector<unsigned char> pixels = g.getPixels();

	GraphicsInterface gui(argc, argv);
	gui.createWindow("gff",600,400,100,100);

	gui.addMenuItem("Wyjscie", GraphicsInterface::EXIT);
	gui.addMenu();
	gui.drawImage(pixels);
	gui.runGUI();

	return 0;
}
