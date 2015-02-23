/*
 * bmpFormat.h
 *
 *  Created on: 16 sty 2015
 *      Author: kama
 */

#ifndef BMPFORMAT_H_
#define BMPFORMAT_H_

#include "GraphicsFormat.h"
#include <stdexcept>
#include <iostream>
#include <stdint.h>
#include <vector>
#include <string>

class BmpFormat: public GraphicsFormat {
public:
	BmpFormat(){
		this->fileSize = 0;
		this->imageWidth = 0;
		this->imageHeight = 0;

	}

	void loadGff(const char* source){
		try{
			this->fileHeader = this->loadHeader(source, 35);

			if(this->fileHeader[0] != 'G' && this->fileHeader[1] != 'F' && this->fileHeader[2] != 'F'){
				throw std::invalid_argument("Niepoprawny format pliku: wymagany typ gff");
			}

			if(this->fileHeader[26] != 24){
				throw std::invalid_argument("Niepoprawny format pliku: obraz musi miec 24bity na pixel");
			}

			this->colorSpace.push_back(this->fileHeader[7]);
			this->colorSpace.push_back(this->fileHeader[8]);
			this->colorSpace.push_back(this->fileHeader[9]);

			this->bitsPerPixel = this->fileHeader[26];
			this->fileSize = (this->fileHeader[3] | this->fileHeader[4]<<8 | this->fileHeader[5]<<16 | this->fileHeader[6]<<24);
			this->imageWidth = this->fileHeader[18] + (this->fileHeader[19]<<8);
			this->imageHeight = this->fileHeader[22] + (this->fileHeader[23]<<8);
			this->pixelOffset = this->fileHeader[10] + (this->fileHeader[11]<<8);
			this->pixelDataSize = (this->fileHeader[31] | this->fileHeader[32]<<8 | this->fileHeader[33]<<16 | this->fileHeader[34]<<24);

			cout<<(int)this->fileSize<<endl;
			cout<<(int)this->imageWidth<<endl;
			cout<<(int)this->imageHeight<<endl;
			cout<<(int)this->pixelOffset<<endl;
			cout<<(int)this->pixelDataSize<<endl;

			this->loadPixels(this->pixelDataSize,this->pixelOffset, false);
			this->decompressData();

		}catch(std::invalid_argument& e){
			this->closeImage();
			std::cout<<"Error: "<<e.what()<<std::endl;

		}catch(std::runtime_error& e){
			this->closeImage();
			std::cout<<"Runtime error: "<<e.what()<<std::endl;
		}
	}

	void saveBmp(const char* path){
		cout<<"swap color"<<endl;
		this->decompressData();
		this->pixelsData = ColorSettings::swapRedBlue(this->pixelsData);


		cout<<"create header"<<endl;
		this->createBmpHeader();
		cout<<"save file"<<endl;
		this->saveFile(path, false);

	}
	void createBmpHeader(){
		this->fileHeader.clear();

		this->headerSize = 40;
		//this->imageSize = ((this->imageWidth * this->bitsPerPixel + 31) / 32) * 4 * this->imageHeight;
		this->imageSize = this->pixelsData.size();
		this->fileSize = this->imageSize + this->headerSize;
		this->bitsPerPixel = 24;
		this->pixelOffset = 55;

		this->fileHeader.resize((int)this->headerSize, 0);

		// write format name
		this->fileHeader[0] = 'B';
		this->fileHeader[1] = 'M';

		// write file size
		this->fileHeader[2] = this->fileSize;
		this->fileHeader[3] = this->fileSize>>8;
		this->fileHeader[4] = this->fileSize>>16;
		this->fileHeader[5] = this->fileSize>>24;

		//write colorspace
		/*this->fileHeader[7] = (char)this->colorSpace[0];
		this->fileHeader[8] = (char)this->colorSpace[1];
		this->fileHeader[9] = (char)this->colorSpace[2];*/

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
		this->fileHeader[28] = this->bitsPerPixel;

		//write algorithm compression name
		/*this->fileHeader[30] = 'L';
		this->fileHeader[31] = 'Z';
		this->fileHeader[32] = '7';
		this->fileHeader[33] = '7';*/

		//write image size
		this->fileHeader[34] = this->imageSize;
		this->fileHeader[35] = this->imageSize>>8;
		this->fileHeader[36] = this->imageSize>>16;
		this->fileHeader[37] = this->imageSize>>24;

		for(int i=38; i<40;i++){
			this->fileHeader[i] = 0;
		}

	}
};

#endif /* BMPFORMAT_H_ */
