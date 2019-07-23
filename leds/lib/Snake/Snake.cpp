#include <Arduino.h>
#include "Snake.h"

#ifndef SNAKE_CPP
#define SNAKE_CPP

// class Snake {

	// float p = 0; // you can be between 2 pixels but getPixel will deal with it
	// float v = 0; // velocity
	// float f = 0; // friction
	//
	// double bornMillis = -1;
	// double lifetimeMS = -1;
//	int [] clr = new int[3];
	// int snakeLength = 1; // in pixels
	// boolean _protected = false;
	// boolean _active = false;
	//
	// // what kind of thing is this fucker?  (eg: blinky boy, fader slowly, wwhatever)
	// // maybe also include some notion of trailing off at the edges
	//
	// int mode = 1; // fades away from velcotiy
	// // int mode = 0; // does nothing

Snake::Snake() {
	bornMillis = millis();
	setPixel(0);
	setVelocity(0);
	snakeLength = 1;
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

void Snake::setLifetime(double lifetime) {
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
void Snake::update(double elapsed, int pixelCount) {


	p = 0;
	v = 0;

	// p = (p + ((float) (elapsed / 1000) * v));
	// p *= 1 - ((float) (elapsed / 1000) * f);
	// v *= 1 - ((float) (elapsed / 1000) * f);

	while (p < 0)
		p = pixelCount + p;

	// p = p % pixelCount;

	if (lifetimeMS > 0 && millis() - bornMillis > lifetimeMS) {
		setInactive();
		setPixel(0);
	}

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
	switch(mode) {
		default:
		case 0:
			return channelValue;
			// break;
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
