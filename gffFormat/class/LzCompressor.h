/*
 * lzCompressor.h
 *
 *  Created on: 16 sty 2015
 *      Author: kama
 */

#ifndef LZCOMPRESSOR_H_
#define LZCOMPRESSOR_H_

#include <vector>
#include <stdint.h>
#include <iostream>

using namespace std;

class LzCompressor {
private:
	const unsigned short DICTIONARY_SIZE = 8192;
	const unsigned short BUFFER_SIZE = 256;

	unsigned int indexStartWindow;
	unsigned int indexEndWindow;

	vector <uint8_t> bytesData;
	vector <uint16_t> compressData;

public:
	LzCompressor(){
		this->indexStartWindow = 0;
		this->indexEndWindow = 0;
	}

	vector<uint16_t> compress(vector<uint8_t> bytesData){
		uint16_t offset = 0;
		uint16_t matchLength = 0;
		uint16_t currentByteValue = 0;

		vector<uint16_t> pattern;
		this->bytesData = bytesData;

		int matchPosition = 0;
		int	currentByteIndex = 0;
		int bytesLength = this->bytesData.size()-1;

		while(currentByteIndex < bytesLength){
			this->indexStartWindow = (currentByteIndex - this->DICTIONARY_SIZE >=0) ? currentByteIndex - this->DICTIONARY_SIZE : 0;
			this->indexEndWindow = (currentByteIndex + this->BUFFER_SIZE < bytesLength) ? currentByteIndex + this->BUFFER_SIZE < bytesLength : bytesLength;
			matchLength = 0;
			offset = 0;

			pattern.clear();

			// catch the color int value
			currentByteValue = (uint16_t)this->bytesData[currentByteIndex];

			if(currentByteIndex == 0){
				this->addCompressItem(0,0,currentByteValue);
			}else{
				pattern.push_back(currentByteValue);
				matchPosition = this->searchPattern(this->indexStartWindow, currentByteIndex, pattern);

				if(matchPosition != -1){
					matchLength=1;
					while(matchLength <= this->BUFFER_SIZE){
						pattern.push_back(this->bytesData[currentByteIndex + matchLength]);
						matchPosition = this->searchPattern(this->indexStartWindow, currentByteIndex, pattern);
						if(matchPosition != -1 && currentByteIndex + matchLength < bytesLength){
							matchLength++;
						}else{
							pattern.pop_back();
							break;
						}
					}
					matchPosition = this->searchPattern(this->indexStartWindow, currentByteIndex, pattern)-1;

					currentByteIndex += matchLength;
					offset = (currentByteIndex < (this->DICTIONARY_SIZE + matchLength)) ? currentByteIndex - matchPosition - matchLength : this->DICTIONARY_SIZE - matchLength;

					this->addCompressItem(offset, matchLength, this->bytesData[currentByteIndex]);

				}else{
					this->addCompressItem(0,0,currentByteValue);
				}
			}

			currentByteIndex++;
		}

		return this->compressData;
	}
	vector<uint8_t> unCompress(){
		vector<uint8_t> rawData;

		unsigned int sizeCompresedData = this->compressData.size();
		unsigned int currentItemIndex = 0;
		unsigned short matchLength = 0;
		unsigned short offset = 0;

		while(currentItemIndex < sizeCompresedData){
			if(this->compressData[currentItemIndex+1] == 0){
				rawData.push_back(this->compressData[currentItemIndex+2]);
				currentItemIndex+=3;
			}else{
				matchLength = this->compressData[currentItemIndex+1];
				offset = rawData.size() - this->compressData[currentItemIndex];

				for(int i=0; i<matchLength; i++){
					rawData.push_back(rawData[offset]);
					offset++;
				}
				rawData.push_back(this->compressData[currentItemIndex+2]);
				currentItemIndex+=3;
			}
		}

		return rawData;
	}

	void clear(){
		this->bytesData.clear();
		this->compressData.clear();
	}

	void readCompressData(){
		int size = this->compressData.size();
		cout<<"Compressed size:"<<size<<endl;
		for(int i = 0; i<size; i+=3){
			cout << "<" << this->compressData[i]<<","<<this->compressData[i+1]<<","<<this->compressData[i+2]<<">"<<endl;
		}
	}

private:
	int searchPattern(int start, int end, vector<uint16_t> pattern){
		int position = -1;
		int i = 0;
		int patternSize = pattern.size();
		int searchSize = 0;
		int t = 0;

		vector<uint16_t> search;
		vector<int> match;

		search.reserve((end-start) + patternSize);

		search.push_back(0);
		search.insert(search.end(), pattern.begin(),pattern.end());
		search.push_back(0);
		search.insert(search.end(), this->bytesData.begin()+start, this->bytesData.begin()+end);

		searchSize = search.size();

		match.resize(searchSize+1, 0);

		for(i = 2; i < searchSize; i++){
			while (t > 0 && search[t+1] != search[i]){
				t = match[t];
			}

			if(search[t+1] == search[i]){
				t++;
			}
			match[i] = t;
		}

		for(i = patternSize + 2; i < searchSize; i++ ){
			if( match[i] == patternSize ){
				position = (i-patternSize-patternSize);
			}
		}

		return position;
	}
	void addCompressItem(uint16_t offset, uint16_t length, uint16_t val){
		this->compressData.push_back(offset);
		this->compressData.push_back(length);
		this->compressData.push_back(val);
	}
};

#endif /* LZCOMPRESSOR_H_ */
