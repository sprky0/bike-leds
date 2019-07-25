#include <Arduino.h>

#ifndef SNAKE_H
#define SNAKE_H

// Color is solid
#define SOLID_COLOR_MODE          0
// Color fades down away from velocity
#define INOUT_FADE_COLOR_MODE     1
// Color fades up, then back down
#define OUTINOUT_FADE_COLOR_MODE  2

class Snake {

	public:

		float p = 0; // you can be between 2 pixels but getPixel will deal with it
		float v = 0; // velocity
		float f = 0; // friction

		unsigned long bornMillis = -1;
		unsigned long lifetimeMS = -1;
		int clr[3] = {0,0,0};
		int snakeLength = 1; // in pixels
		boolean _protected = false;
		boolean _active = false;

		// what kind of thing is this fucker?  (eg: blinky boy, fader slowly, wwhatever)
		// maybe also include some notion of trailing off at the edges


		int snakeDisplayMode = SOLID_COLOR_MODE; // fades away from the direction of velcity

		Snake();
		Snake(int startPixel, int lengthInPixels, float velocityPixelsPerSecond, int r, int g, int b);

		void setPixel(int pixel);
		void setVelocity(float velocityPixelsPerSecond);
		void setFriction(float frictionPerSecond);
		void setLifetime(unsigned long lifetime);
		void setActive();
		void setInactive();
		void setProtected();
		void setUnprotected();
		void update(unsigned long elapsed, int pixelCount);
		float getVelocity();
		int getLength();
		int getPixel();
		int chanelValueAt(int pos, int channelValue);
		int getRAt(int pos);
		int getGAt(int pos);
		int getBAt(int pos);
		boolean isProtected();
		boolean isActive();

	// private:

};

#endif
