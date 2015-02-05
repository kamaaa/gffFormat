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
#include <vector>
#include "LzCompressor.h"

class GffFormat:public GraphicsFormat{
private:
	LzCompressor lz;

public:
	GffFormat(){
		this->fileSize = 0;
		this->imageHeight = 0;
		this->imageWidth = 0;

		this->colorSpace = "RGB";
		this->typeCompression = "LZ77";
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
			this->fileSize = (this->fileHeader[2] | this->fileHeader[3]<<8 | this->fileHeader[4]<<16 | this->fileHeader[5]<<24);
			this->imageWidth = this->fileHeader[18] + (this->fileHeader[19]<<8);
			this->imageHeight = this->fileHeader[22] + (this->fileHeader[23]<<8);
			this->pixelOffset = this->fileHeader[10] + (this->fileHeader[11]<<8);
			this->pixelDataSize = ((this->imageWidth * this->bitsPerPixel + 31) / 32) * 4 * this->imageHeight;

			this->pixelsData = this->loadPixels(this->pixelDataSize, this->pixelOffset);

			std::cout<<this->pixelOffset<<std::endl;
			std::cout<<this->pixelDataSize<<std::endl;

		}catch(std::invalid_argument& e){
			this->closeImage();
			std::cout<<"Error: "<<e.what()<<std::endl;

		}catch(std::runtime_error& e){
			this->closeImage();
			std::cout<<"Runtime error: "<<e.what()<<std::endl;
		}

	}

	void saveGff(const char* path){
		this->compressData = lz.compress(this->pixelsData);
		this->createGffHeader();
		this->saveFile(path);

	}
	void createGffHeader(){
		this->fileHeader.clear();
		this->fileHeader.resize(35);

		this->headerSize = 280;
		this->imageSize = this->compressData.size()*16;
		this->fileSize = this->imageSize + this->headerSize;
		this->bitsPerPixel = 24;
		this->pixelOffset = 36;

		// write format name
		this->fileHeader[0] = 'G';
		this->fileHeader[1] = 'F';
		this->fileHeader[2] = 'F';

		// write file size
		this->fileHeader[3] = this->fileSize;
		this->fileHeader[4] = this->fileSize>>8;
		this->fileHeader[5] = this->fileSize>>16;
		this->fileHeader[6] = this->fileSize>>24;

		this->fileHeader[7] = 'R';
		this->fileHeader[8] = 'G';
		this->fileHeader[9] = 'B';


	}

	void setColorspace(const char* colorspace){
		this->colorSpace = colorspace;
	}
	int getWidth(){
		return this->imageWidth;
	}
	int getHeight(){
		return this->imageHeight;
	}
};

#endif /* GFFFORMAT_H_ */
