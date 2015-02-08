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

	static vector <uint16_t> fromRGBtoHSV(vector <uint8_t> pixel) {

			double h,s,v;   //zmienne koloru
			double r2,g2,b2,delta,Cmin; //zmienne dodatkowe
			uint8_t r, g, b;
	        int size = pixel.size();
	        vector <uint16_t> result;

		  	for(int i = 0; i<size; i += 3){
	            r = pixel[i];
	            g = pixel[i+1];
	            b = pixel[i+2];

	            r2 = r/255;
	            g2 = g/255;
	            b2 = b/255;

	            v = max(r2,max(g2,b2));					// wyznaczamy skladowa v=Cmax
	            Cmin = min(r2,min(g2,b2));
	            delta = v - Cmin;

	            if(v == r2) h = 60 * fmod(((g2-b2)/delta), 6);// obliczamy skladowa h
	            if(v == g2) h = 60 * (((b2 - r2) / delta)+  2);
	            if(v == b2) h = 60 * (((r2 - g2)/delta) + 4);
	            if(v == 0)  s = 0;							// obliczamy skladowa s
	                else s = delta / v;

	            result.push_back(h*1000);
	            result.push_back(s*1000);
	            result.push_back(v*1000);

	            for(int j = 0; j<3; j++){
	            cout << result[j] << "; ";
	            }
	        }
	        return result;
	    }

	static vector <uint16_t> fromRGBtoHSL(vector <uint8_t> pixel) {
			double h, s, l;   //zmienne koloru
			double r2, g2, b2, delta, Cmax, Cmin; //zmienne dodatkowe
			uint8_t r, g, b;
	        int size = pixel.size();
	        vector <uint16_t> result;

		  	for(int i = 0; i<size; i += 3){
	            r = pixel[i];
	            g = pixel[i+1];
	            b = pixel[i+2];

	            r2 = r / 255;
	            g2 = g / 255;
	            b2 = b / 255;

	            Cmax = max(r2,max(g2,b2));
	            Cmin = min(r2,min(g2,b2));
	            delta = Cmax - Cmin;

	            if(Cmax == r2) h = 60 * fmod(((g2-b2) / delta), 6);	//obliczamy skladowa h
	            if(Cmax == g2) h = 60 * (((b2 - r2) / delta) + 2);
	            if(Cmax == b2) h = 60 * (((r2 - g2) / delta) + 4);
	            l = (Cmax + Cmin) / 2;   // wyznaczamy skladowa l
	            if(delta == 0) s = 0; //wyznaczamy skladowa s
	                else s = delta/(1 - abs(2 * l - 1));

	            result.push_back(h * 1000);
	            result.push_back(s * 1000);
	            result.push_back(l * 1000);

	            for(int j = 0; j<3; j++){
	            cout << result[j] << "; ";
	            }
		  	}
		  	return result;
		}

	static vector <uint16_t> fromRGBtoYUV(vector <uint8_t> pixel) {
			double y,u,v;   //zmienne koloru
			uint8_t r, g, b;
			int size = pixel.size();
	        vector <uint16_t> result;

		  	for(int i = 0; i<size; i += 3){
	            r = pixel[i];
	            g = pixel[i+1];
	            b = pixel[i+2];

	            y = (r * 0.299) + (g * 0.587) + (b * 0.114);		//wyznaczamy skladowa y
	            u = (r * (-0.169)) + (g*(-0.332)) + (b * 0.500) + 128; //wyznaczamy skladowa u
	            v = (r * 0.500) + (g * (-0.419)) + (b * (0.0813)) + 128; //wyznaczamy skladowa v
	            result.push_back(y );
	            result.push_back(u );
	            result.push_back(v );

	            for(int j = 0; j<3; j++){
	            cout << result[j] << "; ";
	            }
		  	}
		  	return result;
		}

	static vector <uint8_t> fromHSVtoRGB(vector <uint16_t> pixel) {
	        double h, s, v, c, x, m, rp, gp, bp;
	        uint8_t r, g, b;
	        int size = pixel.size();
	        vector <uint8_t> result;

		  	for(int i = 0; i<size; i += 3){
	             h = (double)pixel[i]/1000;
	             s = (double)pixel[i+1]/1000;
	             v = (double)pixel[i+2]/1000;

	            c = v * s;
	            x = c * (1 - abs(fmod((h / 60), 2) - 1));
	            m = v - c;

	            if(h>=0 and h<60){
	                rp = c;
	                gp = x;
	                bp = 0;
	            }
	            if(h>=60 and h<120){
	                rp = x;
	                gp = c;
	                bp = 0;
	            }
	            if(h>=120 and h<180){
	                rp = 0;
	                gp = c;
	                bp = x;
	            }
	            if(h>=180 and h<240){
	                rp = 0;
	                gp = x;
	                bp = c;
	            }
	            if(h>=240 and h<300){
	                rp = x;
	                gp = 0;
	                bp = c;
	            }
	            if(h>=300 and h<360){
	                rp = c;
	                gp = 0;
	                bp = x;
	            }

	            r = (rp + m) * 255;
	            g = (gp + m) * 255;
	            b = (bp + m) * 255;

	            result.push_back(r);
	            result.push_back(g);
	            result.push_back(b);

	            for(int j = 0; j<3; j++){
	                cout << result[j] << "; ";
	            }

	        }
	        return result;
		}

	static vector <uint8_t> fromYUVtoRGB(vector <uint16_t> pixel) {
	        double y, u, v;
	        uint8_t r, g, b;
	        int size = pixel.size();
	        vector <uint8_t> result;

	        for(int i = 0; i<size; i += 3){
	             y = pixel[i];
	             u = pixel[i+1];
	             v = pixel[i+2];


	            r = y + (1.140 * v);
	            g = y - (0.395 * u) - (0.581 * v);
	            b = y + (2.032 * u);

	            result.push_back(r);
	            result.push_back(g);
	            result.push_back(b);

	            for(int j = 0; j<3; j++){
	               cout << result[j] << "; ";
	            }
	        // nie dziala, powinno byc (0, 218, 0)
	        // http://www.mikekohn.net/file_formats/yuv_rgb_converter.php
	        }
	    return result;
	    }

	static vector <uint8_t> fromHSLtoRGB(vector <uint16_t> pixel) {
	        double h, s, l, c, x, m, rp, gp, bp;
	        uint8_t r, g, b;
	        int size = pixel.size();
	        vector <uint8_t> result;

	        for(int i = 0; i<size; i += 3){
	             h = (double)pixel[i]/100;
	             s = (double)pixel[i+1]/100;
	             l = (double)pixel[i+2]/100;

	            c = (1 - abs(2*l - 1)) * s;
	            x = c * (1 - abs(fmod((h / 60), 2) - 1));
	            m = l - c / 2;

	            if(h>=0 and h<60){
	                rp = c;
	                gp = x;
	                bp = 0;
	            }
	            if(h>=60 and h<120){
	                rp = x;
	                gp = c;
	                bp = 0;
	            }
	            if(h>=120 and h<180){
	                rp = 0;
	                gp = c;
	                bp = x;
	            }
	            if(h>=180 and h<240){
	                rp = 0;
	                gp = x;
	                bp = c;
	            }
	            if(h>=240 and h<300){
	                rp = x;
	                gp = 0;
	                bp = c;
	            }
	            if(h>=300 and h<360){
	                rp = c;
	                gp = 0;
	                bp = x;
	            }

	            r = (rp + m)*255;
	            g = (gp + m)*255;
	            b = (bp + m)*255;

	            result.push_back(r);
	            result.push_back(g);
	            result.push_back(b);

	            for(int j = 0; j<3; j++){
	               cout << result[j] << "; ";
	            }
	        }
	        return result;
	    }

	vector <uint16_t> fromHSVtoHSL(vector <uint16_t> pixel){
		vector <uint8_t> tempRGB = ColorSettings::fromHSVtoRGB(pixel);
		return ColorSettings::fromRGBtoHSL(tempRGB);
	}

	vector <uint16_t> fromHSVtoYUV(vector <uint16_t> pixel){
			vector <uint8_t> tempRGB = ColorSettings::fromHSVtoRGB(pixel);
			return ColorSettings::fromRGBtoYUV(tempRGB);
	}

	vector <uint16_t> fromHSLtoHSV(vector <uint16_t> pixel){
			vector <uint8_t> tempRGB = ColorSettings::fromHSLtoRGB(pixel);
			return ColorSettings::fromRGBtoHSV(tempRGB);
	}

	vector <uint16_t> fromHSLtoYUV(vector <uint16_t> pixel){
			vector <uint8_t> tempRGB = ColorSettings::fromHSLtoRGB(pixel);
			return ColorSettings::fromRGBtoYUV(tempRGB);
	}

	vector <uint16_t> fromYUVtoHSV(vector <uint16_t> pixel){
			vector <uint8_t> tempRGB = ColorSettings::fromYUVtoRGB(pixel);
			return ColorSettings::fromRGBtoHSV(tempRGB);
	}

	vector <uint16_t> fromYUVtoHSL(vector <uint16_t> pixel){
			vector <uint8_t> tempRGB = ColorSettings::fromYUVtoRGB(pixel);
			return ColorSettings::fromRGBtoHSL(tempRGB);
		}



};

#endif /* COLORSETTINGS_H_ */
