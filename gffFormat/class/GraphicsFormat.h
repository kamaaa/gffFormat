/*
 * graphicsFormat.h
 *
 *  Created on: 16 sty 2015
 *  Author: team15
 *
 *  Abstract Class graphicsFormat
 *
 */

#ifndef GRAPHICSFORMAT_H_
#define GRAPHICSFORMAT_H_

#include <vector>
#include <stdexcept>
#include <iostream>
#include <stdint.h>
#include <fstream>
#include "LzCompressor.h"
#include <algorithm>

class GraphicsFormat {

protected:
	uint8_t bitsPerPixel;
	uint32_t pixelOffset;

	unsigned int pixelDataSize;
	uint32_t fileSize;
	uint32_t headerSize;
	uint32_t imageSize;
	uint32_t imageWidth;
	uint32_t imageHeight;

	std::string colorSpace;
	std::string typeCompression;

	std::vector<uint8_t> fileHeader;
	std::vector<uint8_t> pixelsData;
	std::ifstream image;
	std::ofstream saveImage;

	LzCompressor lz;

private:

public:
	std::vector<uint8_t> getPixels(){
		return this->pixelsData;
	}
	std::string getColorspace(){
		return this->colorSpace;
	}
	int getWidth(){
		return this->imageWidth;
	}
	int getHeight(){
		return this->imageHeight;
	}

protected:
	/*
	 * Method: loading file header
	 *
	 * Arguments: source - path to file, headerLength - number of bits header
	 * Throws: invalid_argument
	 * Return: vector - raw file header
	 */
	std::vector<uint8_t> loadHeader(const char* source, int headerLength){
		std::vector<uint8_t> rawData;

		this->image.open(source, std::ios::in | std::ios::binary);

		if(!this->image.is_open()){
			throw std::invalid_argument("Nie ma takiego pliku");
		}

		rawData.resize(headerLength);
		this->image.read(reinterpret_cast<char*>(rawData.data()),headerLength);

		return rawData;
	}

	std::vector<uint8_t> loadPixels(int size, int offset){
		std::vector<uint8_t> rawData;

		if(!this->image.is_open()){
			throw std::runtime_error("Plik nie zostal otwarty.");
		}

		rawData.resize(size);

		this->image.seekg(offset, std::ios::beg);
		this->image.read(reinterpret_cast<char*>(rawData.data()),size);

		this->closeImage();
		return rawData;
	}

	void compressData(){
		this->pixelsData = lz.compress(this->pixelsData);
		lz.clear();
	}

	void decompressData(){
		this->pixelsData = lz.decompress(this->pixelsData);
		lz.clear();
	}

	void saveFile(const char* path){
		this->saveImage.open(path, std::ios::binary);

		cout<<(int)this->imageSize<<endl;
		this->saveImage.write(reinterpret_cast<char*>(this->fileHeader.data()), (int)this->headerSize);
		this->saveImage.write(reinterpret_cast<char*>(this->pixelsData.data()), (int)this->imageSize);

		if(this->saveImage.good()){
			cout<<"ok"<<endl;
		}else{
			cout<<"nie ok"<<endl;
		}
		this->saveImage.close();
	}

	void closeImage(){
		this->image.close();
	}
};

#endif /* GRAPHICSFORMAT_H_ */
