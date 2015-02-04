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
	const unsigned short BUFFER_SIZE = 2048;

	unsigned int indexStartWindow;
	unsigned int indexEndWindow;

	vector <unsigned char> bytesData;
	vector <uint16_t> compressData;

public:
	LzCompressor(vector<unsigned char> rowData){
		this->indexStartWindow = 0;
		this->indexEndWindow = 0;
		this->bytesData = rowData;
	}

	void compress(){

		int matchPosition = 0;
		uint16_t offset = 0;
		uint16_t matchLength = 0;
		uint16_t currentByteValue = 0;

		vector<uint16_t> pattern;

		int	currentByteIndex = 0;
		int bytesLength = this->bytesData.size();

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
				//cout<<"Wartość: "<<currentByteValue<<" Na pozycji: "<<matchPosition<<endl;
				//cout<<matchPosition<<endl;
				if(matchPosition != -1){
					matchLength=1;
					while(matchLength <= this->BUFFER_SIZE){
						pattern.push_back((uint16_t)this->bytesData[currentByteIndex + matchLength]);
						matchPosition = this->searchPattern(this->indexStartWindow, currentByteIndex, pattern);
						if(matchPosition != -1 && currentByteIndex + matchLength < bytesLength /*&& (matchPosition + matchLength) < currentByteIndex*/){
							matchLength++;
						}else{
							pattern.pop_back();
							break;
						}
					}
					matchPosition = this->searchPattern(this->indexStartWindow, currentByteIndex, pattern)-1;
					//cout<<"Match len:"<<matchLength<<endl;

					currentByteIndex += matchLength;
					offset = currentByteIndex - matchPosition - matchLength;
					this->addCompressItem(offset, matchLength, this->bytesData[currentByteIndex]);

				}else{
					this->addCompressItem(0,0,currentByteValue);
				}
			}

			currentByteIndex++;
		}
	}
	vector<unsigned char> unCompress(vector<unsigned char>bytes){
		vector<unsigned char> result;

		return result;
	}
	void readCompress(){
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
