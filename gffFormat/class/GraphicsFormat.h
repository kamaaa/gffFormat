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

class GraphicsFormat {

protected:
	short bitsPerPixel;
	uint32_t pixelOffset;

	int imageSize;
	int imageWidth;
	int imageHeight;

	std::vector<uint8_t> fileHeader;
	std::vector<unsigned char> pixelsData;
	std::ifstream image;

private:

public:
	std::vector<unsigned char> getPixels(){
		return this->pixelsData;
	}

protected:
	/*
	 * Method: loading file header
	 *
	 * Arguments: source - path to file, headerLength - number of bits header
	 * Throws: invalid_argument
	 * Return: vector - row file header
	 */
	std::vector<uint8_t> loadHeader(const char* source, int headerLength){
		std::vector<uint8_t> rowData;

		this->image.open(source, std::ios::in | std::ios::binary);
		if(!this->image.is_open()){
			throw std::invalid_argument("Nie ma takiego pliku");
		}

		rowData.resize(headerLength);
		this->image.read(reinterpret_cast<char*>(rowData.data()),headerLength);

		return rowData;
	}

	std::vector<unsigned char> loadPixels(int size, int offset){
		std::vector<unsigned char> rowData;

		if(!this->image.is_open()){
			throw std::runtime_error("Plik nie zostal otwarty.");
		}

		rowData.resize(size);

		this->image.seekg(offset, std::ios::beg);
		this->image.read(reinterpret_cast<char*>(rowData.data()),size);

		this->closeImage();
		return rowData;
	}

	void closeImage(){
		this->image.close();
	}
};

#endif /* GRAPHICSFORMAT_H_ */
