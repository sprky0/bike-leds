#include <Arduino.h>
#include "Snake.h"

#ifndef SNAKE_CPP
#define SNAKE_CPP

Snake::Snake() {

	bornMillis = millis();

	setPixel(0);
	setVelocity(0);
	snakeLength = 1;

	// Default to 0,0,0 black
	clr[0] = 0;
	clr[1] = 0;
	clr[2] = 0;
}

Snake::Snake(int startPixel, int lengthInPixels, float velocityPixelsPerSecond, int r, int g, int b) {

	bornMillis = millis();

	// what pixel are we 'on'
	setPixel(startPixel);
	setVelocity(velocityPixelsPerSecond);
	snakeLength = lengthInPixels;

	// Set color to r,g,b
	clr[0] = r;
	clr[1] = g;
	clr[2] = b;

}

void Snake::setPixel(int pixel) {
	p = pixel;
}

void Snake::setVelocity(float velocityPixelsPerSecond) {
	v = velocityPixelsPerSecond;
}

void Snake::setFriction(float frictionPerSecond) {
	f = frictionPerSecond;
}

void Snake::setLifetime(unsigned long lifetime) {
	lifetimeMS = lifetime;
}

void Snake::setActive() {
	_active = true;
}

void Snake::setInactive() {
	_active = false;
}

void Snake::setProtected() {
	_protected = true;
}

void Snake::setUnprotected() {
	_protected = false;
}

// passing elapsed time so we don't need to internally calculate it on each snake
void Snake::update(unsigned long elapsed, int pixelCount) {

	float elapsedFloatMS = (float) elapsed;
	float pixelCountF = (float) pixelCount;

	// Serial.println( elapsedFloatMS );

	// p = 0;
	// v = 0;

	p = (p + ((elapsedFloatMS / 1000) * v));
	// p *= 1 - ((float) (elapsed / 1000) * f);
	// v *= 1 - ((float) (elapsed / 1000) * f);
	//
	// while (p < 0)
	// 	p = pixelCountF + p;
	// p = p % pixelCountF;

	if (p < 0) {
		p = pixelCountF - 1;
	} else if (p > pixelCountF) {
		p = 0;
	}

	if (lifetimeMS > 0 && millis() - bornMillis > lifetimeMS) {
		Serial.println("PIXEL DONE DIED");
		setInactive();
		setPixel(0);
	}

	// Serial.print(elapsedFloatMS);
	// Serial.print(" ");
	// Serial.print(v);
	// Serial.print(" ");
	// Serial.println(p);

}

float Snake::getVelocity() {
	return v;
}

int Snake::getLength() {
	return snakeLength;
}

int Snake::getPixel() {
	return (int) p;
}

int Snake::chanelValueAt(int pos, int channelValue) {
	switch(snakeDisplayMode) {
		default:
		case 0:
			return channelValue;
		case 1:
			float scaleValue = ((float) snakeLength - (float) pos) / (float) snakeLength;
			if (v > 0) {
				scaleValue = 1 - scaleValue;
			}
			float channelScaledValue = scaleValue * (float) channelValue;
			return (int) channelScaledValue;
	}
}

// pos = 0 to (length-1)
int Snake::getRAt(int pos) {
	return chanelValueAt(pos, clr[0]);
}

int Snake::getGAt(int pos) {
	return chanelValueAt(pos, clr[1]);

}

int Snake::getBAt(int pos) {
	return chanelValueAt(pos, clr[2]);
}

boolean Snake::isProtected() {
	return _protected;
}

boolean Snake::isActive() {
	return _active;
}

#endif
