#include <Arduino.h>
#include "PixelProxy.h"

#ifndef PIXELPROXY_CPP
#define PIXELPROXY_CPP

/**
 * Class that simulates approximately what NeoPixel lib has for its API
 * Also draws the thing w/ .show()
 */
// class PixelProxy {

	// in .h
	// int brightness = 255;
	// int [300][3] pixels;
	// int pixelCount = 0;

	// there's also pin and some other crap in the constructor for NeoPixel library
PixelProxy::PixelProxy() {
	// ok so change of plans -- too specific implementation, i am hard coding pixel counts etc
	// pixelCount = num;
	// initialize this fucking bastard
	pixels = new int[pixelCount][3];
}

void PixelProxy::setBrightness(int b) {
	// if (b < 0 || b > 255)
	// 	throw(new Exception("Brightness out of range"));
	brightness = b;
}

void PixelProxy::show() {

	// in here we have to deal with applying LED shit to actual LED strip
	// and deal with like parallel strips that are the 'same' and all this sort of shit



	// int w = width / pixelCount;
	// int h = height;
	// for(int i = 0; i < pixelCount; i++) {
	// 	// // pixels[i]
	// 	// noStroke();
	// 	// fill(pixels[i][0],pixels[i][1],pixels[i][2]);
	// 	// rect(i * w, 0, w, h);
	// }
}

void PixelProxy::setPixelColor(int pixel, int r, int g, int b) {

	// this was to ensure we don't get out of bounds Exceptions
	// if (pixel >= 0 && pixel < totalPixelCount) {
	pixels[pixel][0] = r;
	pixels[pixel][1] = g;
	pixels[pixel][2] = b;
	// }
}

void PixelProxy::setPixelColorAdditive(int pixel, int r, int g, int b) {

	// if (pixel >= 0 && pixel < totalPixelCount) {

		pixels[pixel][0] = pixels[pixel][0] + r;
		pixels[pixel][1] = pixels[pixel][1] + g;
		pixels[pixel][2] = pixels[pixel][2] + b;

		if (pixels[pixel][0] > 255)
			pixels[pixel][0] = 255;

		if (pixels[pixel][1] > 255)
			pixels[pixel][1] = 255;

		if (pixels[pixel][2] > 255)
			pixels[pixel][2] = 255;

	// }
}

#endif
