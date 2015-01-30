/*
 * colorSettings.h
 *
 *  Created on: 16 sty 2015
 *  Author: team15
 */

#ifndef COLORSETTINGS_H_
#define COLORSETTINGS_H_

#include<algorithm>

class ColorSettings {
public:
	static const short RGB = 0;
	static const short BGR = 1;
	static const short HSL = 2;
	static const short HSV = 3;
	static const short YUV = 4;

	ColorSettings(){

	}

	static std::vector<unsigned char> swapRedBlue(std::vector<unsigned char> bytes){
		int length = bytes.size();
		for(int i=0; i<length; i+=3){
			std::swap(bytes[i],bytes[i+2]);
		}
		return bytes;
	}
};

#endif /* COLORSETTINGS_H_ */
