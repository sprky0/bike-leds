#include <Arduino.h>

#ifndef PIXELPROXY_H
#define PIXELPROXY_H

/**
 * Class that simulates approximately what NeoPixel lib has for its API
 * Also draws the thing w/ .show()
 */
class PixelProxy {

	public:

		int brightness = 255;
		int pixels[300][3];
		int pixelCount = 300;

		// there's also pin and some other crap in the constructor for NeoPixel library
		PixelProxy();

		void setBrightness(int b);
		void show();
		void setPixelColor(int pixel, int r, int g, int b);
		void setPixelColorAdditive(int pixel, int r, int g, int b);

};

#endif
