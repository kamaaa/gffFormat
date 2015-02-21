/*
 * main.cpp
 *
 * Created on: 14 sty 2015
 * Author: team15
 *
 */

#include <iostream>
#include <bitset>
#include <gtkmm-3.0/gtkmm.h>
#include "class/Stage.h"
//#include "class/GraphicsInterface.h"
//#include "class/GffFormat.h"
//#include "class/LzCompressor.h"

using namespace std;

int main(int argc, char *argv[]){
	/*GraphicsInterface gi(argc, argv);
	GffFormat gff;
	LzCompressor lz;
	vector<unsigned char> imageData;

	gff.loadBitmap("/home/kama/Pobrane/beview_small.bmp");
	imageData = gff.getPixels();
	//imageData = lz.compress(imageData);
	//imageData = lz.decompress(imageData);

	gi.createWindow("OpenGL test", 700, 500, 50, 50);
	gi.drawImage(imageData,gff.getWidth(),gff.getHeight());
	gi.runGUI();*/

	Stage stage(argc, argv, "GFF Format", 700, 500);
	stage.addMainMenu();

	stage.addStage();

	return 0;
}
