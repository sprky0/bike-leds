#include <Arduino.h>
#include "PixelProxy.h"

#ifndef PRACTICAL_PIXEL_COUNT
#define PRACTICAL_PIXEL_COUNT 260
#endif

#ifndef PIXELPROXY_CPP
#define PIXELPROXY_CPP

PixelProxy::PixelProxy() {}

void PixelProxy::setPixelColor(int pixel, int r, int g, int b) {

	if (pixel >= 0 && pixel < PRACTICAL_PIXEL_COUNT) {

		pixels[pixel][0] = r;
		pixels[pixel][1] = g;
		pixels[pixel][2] = b;

	}

}

void PixelProxy::setAll(int r, int g, int b) {
	for(int i = 0; i < PRACTICAL_PIXEL_COUNT; i++) {
		setPixelColor(i,r,g,b);
	}
}

void PixelProxy::setPixelColorAdditive(int pixel, int r, int g, int b) {

	if (pixel >= 0 && pixel < PRACTICAL_PIXEL_COUNT) {

		pixels[pixel][0] = pixels[pixel][0] + r;
		pixels[pixel][1] = pixels[pixel][1] + g;
		pixels[pixel][2] = pixels[pixel][2] + b;

		if (pixels[pixel][0] > 255)
			pixels[pixel][0] = 255;

		if (pixels[pixel][1] > 255)
			pixels[pixel][1] = 255;

		if (pixels[pixel][2] > 255)
			pixels[pixel][2] = 255;

	}

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
