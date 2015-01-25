/*
 * gffFormat.h
 *
 *  Created on: 16 sty 2015
 *      Author: kama
 */

#ifndef GFFFORMAT_H_
#define GFFFORMAT_H_

#include "GraphicsFormat.h"
#include <stdexcept>
#include <iostream>
#include <stdint.h>

class GffFormat:public GraphicsFormat{

public:
	GffFormat(){
		this->imageSize = 0;
		this->imageHeight = 0;
		this->imageWidth = 0;
	}
	/*
	 * Method: load Bitmap from file
	 *
	 * Arguments: source - path to file
	 * Throws: invalid_argument if file is not bmp
	 * Return: void
	 */
	void loadBitmap(const char* source){
		try{
			this->fileHeader = this->loadHeader(source, 54);

			if(this->fileHeader[0] != 'B' && this->fileHeader[1] != 'M'){
				throw std::invalid_argument("Niepoprawny format pliku: wymagany typ bmp");
			}

			if(this->fileHeader[28] != 24){
				throw std::invalid_argument("Niepoprawny format pliku: obraz musi miec 24bity na pixel");
			}

			this->bitsPerPixel = this->fileHeader[28];
			this->imageSize = (this->fileHeader[2] | this->fileHeader[3]<<8 | this->fileHeader[4]<<16 | this->fileHeader[5]<<24);
			this->imageWidth = this->fileHeader[18] + (this->fileHeader[19]<<8);
			this->imageHeight = this->fileHeader[22] + (this->fileHeader[23]<<8);
			this->pixelOffset = this->fileHeader[10] + (this->fileHeader[11]<<8);

			if(this->imageSize == 0){
				this->imageSize = ((this->imageWidth * this->bitsPerPixel + 31) / 32) * 4 * this->imageHeight;
			}

			this->pixelsData = this->loadPixels(this->imageSize, this->pixelOffset);

			std::cout<<this->pixelOffset<<std::endl;
			std::cout<<this->imageSize<<std::endl;

		}catch(std::invalid_argument& e){
			this->closeImage();
			std::cout<<"Error: "<<e.what()<<std::endl;

		}catch(std::runtime_error& e){
			this->closeImage();
			std::cout<<"Runtime error: "<<e.what()<<std::endl;
		}

	}
	int getWidth(){
		return this->imageWidth;
	}
	int getHeight(){
		return this->imageHeight;
	}
};

#endif /* GFFFORMAT_H_ */
