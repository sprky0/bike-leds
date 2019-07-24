#include <Arduino.h>
#include "PixelProxy.h"

#ifndef PIXELPROXY_CPP
#define PIXELPROXY_CPP

// there's also pin and some other crap in the constructor for NeoPixel library
PixelProxy::PixelProxy() {
	// pixels = int[STRIP_PIXEL_LENGTH][3];
}

void PixelProxy::setBrightness(int b) {
	if (b >= 0 && b < 255)
		brightness = b;
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

int PixelProxy::getRAt(int pixel) {
	return pixels[pixel][0];
}
int PixelProxy::getGAt(int pixel) {
	return pixels[pixel][1];
}
int PixelProxy::getBAt(int pixel) {
	return pixels[pixel][2];
}

#endif
