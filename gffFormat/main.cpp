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

	Stage stage(argc, argv, "GFF Format", 700, 500);
	stage.addMainMenu();

	stage.addStage();



	return 0;
}
