/*
 * main.cpp
 *
 * Created on: 14 sty 2015
 * Author: team15
 *
 */

#include <iostream>
#include <vector>
#include <gtkmm-3.0/gtkmm.h>
#include "class/Stage.h"

using namespace std;

int main(int argc, char *argv[]){
	Stage stage(argc, argv, "GFF Format", 600, 400);
	stage.addMainMenu();

	stage.addStage();

	return 0;
}
