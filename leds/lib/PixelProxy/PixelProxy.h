#include <Arduino.h>

#ifndef STRIP_PIXEL_LENGTH
#define STRIP_PIXEL_LENGTH 10
#endif

#ifndef PIXELPROXY_H
#define PIXELPROXY_H

/**
 * Class that simulates approximately what NeoPixel lib has for its API
 * Also draws the thing w/ .show()
 */
class PixelProxy {

	public:

		int brightness = 255;
		int pixels[STRIP_PIXEL_LENGTH][3];
		int pixelCount = STRIP_PIXEL_LENGTH;

		// there's also pin and some other crap in the constructor for NeoPixel library
		PixelProxy();

		void setBrightness(int b);
		void setPixelColor(int pixel, int r, int g, int b);
		void setPixelColorAdditive(int pixel, int r, int g, int b);

		int getRAt(int pixel);
		int getGAt(int pixel);
		int getBAt(int pixel);

};

#endif
