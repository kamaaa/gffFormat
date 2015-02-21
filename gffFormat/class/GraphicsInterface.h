/*
 * graphicsInterface.h
 *
 *  Created on: 16 sty 2015
 *  Author: team15
 */


#ifndef GRAPHICSINTERFACE_H_
#define GRAPHICSINTERFACE_H_

#include <GL/freeglut.h>
#include <vector>

class GraphicsInterface {

private:
	std::vector<const char*> menuItemName;
	std::vector<int> menuItemId;
	int stageWidth;
	int stageHeight;
	std::vector<unsigned char> pixels;
	int imageWidth;
	int imageHeight;

public:
	static const int EXIT = 0;


	/* Init openGL */
	GraphicsInterface(int argc,char **argv){
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
		this->stageWidth = 0;
		this->stageHeight = 0;
	}

	/*
	 * Medhod: Create a program graphics interface
	 * Arguments: title - window name, width - window width, height - window height, x - window x position(default 100), y - window y position(default 100)-
	 *
	 * Returns: void
	 */
	void createWindow(const char* title, int width, int height, int x=100, int y=100){
		this->stageWidth = width;
		this->stageHeight = height;

		glutInitWindowSize(width, height);
		glutInitWindowPosition(x, y);
		glutCreateWindow(title);

		// obszar renderingu - całe okno
		glViewport(0, 0, this->stageWidth, this->stageHeight);

		// wybór macierzy rzutowania
		glMatrixMode( GL_PROJECTION );

		// macierz rzutowania = macierz jednostkowa
		glLoadIdentity();

		// rzutowanie prostokątne
		gluOrtho2D( 0, width, 0, height );

	}
	void addMenu(){
		glutCreateMenu(this->attachMenuEvent);
		this->attachMenuItems();
		glutAttachMenu(GLUT_RIGHT_BUTTON);
	}
	void addMenuItem(const char* name, int id){
		this->menuItemName.push_back(name);
		this->menuItemId.push_back(id);
	}
	void drawImage(std::vector<unsigned char> pixels, int width, int height){
		this->pixels = pixels;
		this->imageWidth = width;
		this->imageHeight = height;

	}
	void runGUI(){
		this->InitializeGlutCallbacks();

		glutMainLoop();
	}
	void Render(){
		//Background color
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glRasterPos2i(0,0);

		glDrawPixels(this->imageWidth,this->imageHeight, GL_BGR, GL_UNSIGNED_BYTE, this->pixels.data());
		glFinish();

		glColor3f(0,0,0);
		glRasterPos2i(10,glutGet(GLUT_WINDOW_HEIGHT)-20);
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char*)"Koala.bmp");
		glFlush();

		glColor3f(255,255,255);
		glRecti(50,50,100,100);
		glFlush();

		glutSwapBuffers();
	}
private:
	void InitializeGlutCallbacks(){
		prepareRender();
	}

	void attachMenuItems(){
		int length = this->menuItemId.size();
		for(int i=0; i<length; i++){
			glutAddMenuEntry(this->menuItemName[i], this->menuItemId[i]);
		}
	}

	static void attachMenuEvent(int buttonID){
		switch(buttonID){
		case GraphicsInterface::EXIT:

			exit(0);
		}

	}
	void prepareRender();

};

GraphicsInterface *gi;
extern "C" void drawCallback(){
	gi->Render();
}

void GraphicsInterface::prepareRender(){
	::gi = this;
	::glutDisplayFunc(drawCallback);
}

#endif /* GRAPHICSINTERFACE_H_ */
