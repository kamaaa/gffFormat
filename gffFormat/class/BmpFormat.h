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

			this->pixelsData = lz.decompress(this->loadPixels(this->pixelDataSize,this->pixelOffset));

		}catch(std::invalid_argument& e){
			this->closeImage();
			std::cout<<"Error: "<<e.what()<<std::endl;

		}catch(std::runtime_error& e){
			this->closeImage();
			std::cout<<"Runtime error: "<<e.what()<<std::endl;
		}
	}
};

#endif /* BMPFORMAT_H_ */
