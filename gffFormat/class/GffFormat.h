/*
 * gffFormat.h
 *
 *  Created on: 16 sty 2015
 *      Author: kama
 */

#ifndef GFFFORMAT_H_
#define GFFFORMAT_H_

#include "GraphicsFormat.h"
#include "ColorSettings.h"
#include <stdexcept>
#include <iostream>
#include <stdint.h>
#include <vector>

class GffFormat:public GraphicsFormat{
private:

public:
	GffFormat(){
		this->fileSize = 0;
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

			this->colorSpace = "BGR";
			this->bitsPerPixel = this->fileHeader[28];
			this->fileSize = (this->fileHeader[2] | this->fileHeader[3]<<8 | this->fileHeader[4]<<16 | this->fileHeader[5]<<24);
			this->imageWidth = this->fileHeader[18] + (this->fileHeader[19]<<8);
			this->imageHeight = this->fileHeader[22] + (this->fileHeader[23]<<8);

			this->pixelOffset = this->fileHeader[10] + (this->fileHeader[11]<<8);
			this->pixelDataSize = ((this->imageWidth * this->bitsPerPixel + 31) / 32) * 4 * this->imageHeight;

			this->loadPixels(this->pixelDataSize, this->pixelOffset, true);

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
		cout<<"swap color"<<endl;
		this->pixelsData = ColorSettings::swapRedBlue(this->pixelsData);

		cout<<"compress data"<<endl;
		this->compressData();
		cout<<"create header"<<endl;
		this->createGffHeader();
		cout<<"save file"<<endl;
		this->saveFile(path, true);

	}
	void createGffHeader(){
		this->fileHeader.clear();

		this->headerSize = 35;
		this->imageSize = this->compressPixelData.size()*2;
		this->fileSize = this->imageSize + this->headerSize;
		this->bitsPerPixel = 24;
		this->pixelOffset = 35;

		this->fileHeader.resize((int)this->headerSize);

		// write format name
		this->fileHeader[0] = 'G';
		this->fileHeader[1] = 'F';
		this->fileHeader[2] = 'F';

		// write file size
		this->fileHeader[3] = this->fileSize;
		this->fileHeader[4] = this->fileSize>>8;
		this->fileHeader[5] = this->fileSize>>16;
		this->fileHeader[6] = this->fileSize>>24;

		//write colorspace
		this->fileHeader[7] = (char)this->colorSpace[0];
		this->fileHeader[8] = (char)this->colorSpace[1];
		this->fileHeader[9] = (char)this->colorSpace[2];

		//write offset
		this->fileHeader[10] = this->pixelOffset;
		this->fileHeader[11] = this->pixelOffset>>8;
		this->fileHeader[12] = this->pixelOffset>>16;
		this->fileHeader[13] = this->pixelOffset>>24;

		//write header size
		this->fileHeader[14] = this->headerSize;
		this->fileHeader[15] = this->headerSize>>8;
		this->fileHeader[16] = this->headerSize>>16;
		this->fileHeader[17] = this->headerSize>>24;

		//write image width
		this->fileHeader[18] = this->imageWidth;
		this->fileHeader[19] = this->imageWidth>>8;
		this->fileHeader[20] = this->imageWidth>>16;
		this->fileHeader[21] = this->imageWidth>>24;

		//write image height
		this->fileHeader[22] = this->imageHeight;
		this->fileHeader[23] = this->imageHeight>>8;
		this->fileHeader[24] = this->imageHeight>>16;
		this->fileHeader[25] = this->imageHeight>>24;

		//write bits per pixel
		this->fileHeader[26] = this->bitsPerPixel;

		//write algorithm compression name
		this->fileHeader[27] = 'L';
		this->fileHeader[28] = 'Z';
		this->fileHeader[29] = '7';
		this->fileHeader[30] = '7';

		//write image size
		this->fileHeader[31] = this->imageSize;
		this->fileHeader[32] = this->imageSize>>8;
		this->fileHeader[33] = this->imageSize>>16;
		this->fileHeader[34] = this->imageSize>>24;

	}

	void setColorspace(string colorspace){
		this->colorSpace = colorspace;
	}
};

#endif /* GFFFORMAT_H_ */
