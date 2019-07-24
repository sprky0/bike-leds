// @todo reconcile this: we have some dupes and crap

#define TOTAL_PIXEL_COUNT 300
#define PRACTICAL_PIXEL_COUNT 300
#define MAX_SNAKES 300
#define STRIP_PIXEL_LENGTH 300
#define PIXEL_PIN 6


#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "PixelProxy.h"
#include "Snake.h"


int mode = 0;

//
PixelProxy proxy = PixelProxy();
//
double previousMillis = 0;
double cycleMillis = 0;
//
//
// int maxSnakes = 300; // we don't really need more than we have pixels right ?
// int snakeCount = 0; //
//
int minR = 100;
int maxR = 255;
int minG = 0;
int maxG = 0;
int minB = 0;
int maxB = 255;

Adafruit_NeoPixel strip(STRIP_PIXEL_LENGTH, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
Snake snakes[MAX_SNAKES]; //  = Snake[MAX_SNAKES];
//
void setup() {

	// reserve memory for snakes
	populateSnakes();

	// set up strip proxy - the thing we directly interact with to set 'pixels' along the range
	// should be done above before setup
	// proxy.setXYZ whatever

	// set up actual Adafruit LED library - the thing we 'print' to by asking PixelProxy for values
	// strip = new ...etc...

	strip.begin(); // Initialize NeoPixel strip object (REQUIRED)

	for(int i = 0; i < STRIP_PIXEL_LENGTH; i++) {
		strip.setPixelColor(i , 0, 0, 0);
	}

	strip.show();  // Initialize all pixels to 'off'

}
//
void loop() {

	double elapsed = millis() - previousMillis;

	updateDisplay(elapsed);

	/*
	switch(mode) {

		// 1225

		case 1223:
			if (cycleMillis > 500) {
				// explodeSnake();
				int snakeIndex = getFreeSnakeIndex();
				snakes[snakeIndex] = Snake(
					0,
					100,
					500,
					(int) random(minR, maxR),
					(int) random(minG, maxG),
					(int) random(minB, maxB)
				);
				snakes[snakeIndex].setActive();
				snakes[snakeIndex].setLifetime(10000);

				cycleMillis = 0;
			}
		break;

		case 1224:
			if (cycleMillis > 500) {
				// explodeSnake();
				int snakeIndex = getFreeSnakeIndex();
				snakes[snakeIndex] = Snake(
					0,
					8,
					25,
					(int) random(minR, maxR),
					(int) random(minG, maxG),
					(int) random(minB, maxB)
				);
				snakes[snakeIndex].setActive();
				snakes[snakeIndex].setLifetime(10000);

				snakeIndex = getFreeSnakeIndex();
				snakes[snakeIndex] = Snake(
					0,
					2,
					50,
					(int) random(minR, maxR),
					(int) random(minG, maxG),
					(int) random(minB, maxB)
				);
				snakes[snakeIndex].setActive();
				snakes[snakeIndex].setLifetime(10000);

				snakeIndex = getFreeSnakeIndex();
				snakes[snakeIndex] = Snake(
					0,
					1,
					100,
					(int) random(minR, maxR),
					(int) random(minG, maxG),
					(int) random(minB, maxB)
				);
				snakes[snakeIndex].setActive();
				snakes[snakeIndex].setLifetime(10000);

				cycleMillis = 0;
			}
		break;

		// default:
		case 1111:

			// switch mode outside this matybe ? diff modes have diff timing i guess
			if (cycleMillis > 250) {

				addARandomSnake();

				if (random(0,1) > 0.5) {
					int sploder = getActiveSnakeIndex();
					if (sploder > 0)
						explodeSnake(sploder);
				}

				cycleMillis = 0;
			}

		break;

	}
	*/
	previousMillis = millis();
	cycleMillis += elapsed;

	// updateDisplay();

}

void updateDisplay(double elapsed) {

	// set all default to black
	for(int i = 0; i < PRACTICAL_PIXEL_COUNT; i++) {
		proxy.setPixelColor(i,0,0,0);
	}

	// set all to pixel color from
	for(int i = 0; i < MAX_SNAKES; i++) {

		if (!snakes[i].isActive())
			continue;

		int startP = snakes[i].getPixel();

		for (int j = 0; j < snakes[i].getLength(); j++) {
			// proxy.setPixelColor((startP + j) % PRACTICAL_PIXEL_COUNT, snakes[i].getRAt(j), snakes[i].getGAt(j), snakes[i].getBAt(j));
			proxy.setPixelColorAdditive((startP + j) % PRACTICAL_PIXEL_COUNT, snakes[i].getRAt(j), snakes[i].getGAt(j), snakes[i].getBAt(j));
		}

		snakes[i].update(elapsed, PRACTICAL_PIXEL_COUNT);

	}

	// set all default to black
	for(int i = 0; i < PRACTICAL_PIXEL_COUNT; i++) {
		strip.setPixelColor(i, proxy.getGAt(i), proxy.getRAt(i), proxy.getBAt(i));
	}

	strip.show();

}

int getActiveSnakeIndex() {
	// try 10x to get a random nonprotected snake
	int tries = 0;
	while (tries < MAX_SNAKES) {
		int snakeTry = (int) random(0,MAX_SNAKES);
		if (!snakes[snakeTry].isProtected() && snakes[snakeTry].isActive()) {
			return snakeTry;
		}
		tries ++;
	}
	// we totally fail:
	return -1;
}

int getFreeSnakeIndex() {
	// first try to find the first Inactive snake
	for(int i = 0; i < MAX_SNAKES; i++) {
		if (!snakes[i].isActive()) {
			return i;
		}
	}

	// try 10x to get a random nonprotected snake
	int tries = 0;
	while (tries < 10) {
		int snakeTry = (int) random(0,MAX_SNAKES);
		if (!snakes[snakeTry].isProtected()) {
			return snakeTry;
		}
		tries ++;
	}
	// we totally fail:
	return -1;
}

void populateSnakes() {
	for(int i = 0; i < MAX_SNAKES; i++) {
		snakes[i] = Snake(0, 1, 0, 0, 0, 0);
	}
}

void explodeSnake(int snakeIndex) {

	int startPixel = snakes[snakeIndex].getPixel();
	int endPixel = snakes[snakeIndex].getLength();
	float velocityBase = snakes[snakeIndex].getVelocity();

	for(int i = 0; i < 6; i++) {
		int snakeIndexTemporary = getFreeSnakeIndex();
		if (snakeIndexTemporary > 0) {
			snakes[snakeIndexTemporary] = Snake(
				(int) random(startPixel - 2, endPixel + 2),// starting pixel
				1,// length
				// speed
				velocityBase + random(-40,40),
				// RGB:
				255,
				127,
				0
			);
			snakes[snakeIndexTemporary].setActive();
			snakes[snakeIndexTemporary].setLifetime( (int) random(500,1000) );
		}
	}

	snakes[snakeIndex].setLifetime(300);
	// snakes[snakeIndex].setInactive();
	// snakes[snakeIndex].setPixel(0);

}

void addARandomSnake() {

	int snakeIndex = getFreeSnakeIndex();

	if (snakeIndex >= 0) {

		snakes[snakeIndex] = Snake(
			0,// starting pixel
			(int) random(0,10),// length
			// speed
			random(-100,100),
			// RGB:
			(int)random(0,255),(int)random(0,255),(int)random(0,255)
		);
		snakes[snakeIndex].setActive();
		snakes[snakeIndex].setLifetime( (int) random(1000,5000) );
	}

}

void regularSnakes() {

	int curPixel = 0;

	int _r = (int) random(0,255);
	int _g = (int) random(0,255);
	int _b = (int) random(0,255);
	int _dirR = (int) 1;
	int _dirG = (int) 1;
	int _dirB = (int) 1;

	int snakeLength = (int) random(1,10);
	int snakeSpacing = snakeLength * (int) random(2,10);


	for(int i = 0; i < MAX_SNAKES; i++) {

		snakes[i] = Snake(
			// starting pixel
			curPixel,
			// length
			snakeLength,
			// speed
			100,
			// RGB:
			_r,_g,_b
		);

		_r+=_dirR;
		_g+=_dirG;
		_b+=_dirB;

		if (_r > 255) {
			_r = 255;
			_dirR = -1;
		}
		if (_g > 255) {
			_g = 255;
			_dirG = -1;
		}
		if (_b > 255) {
			_b = 255;
			_dirB = -1;
		}
		if (_r < 0) {
			_r = 0;
			_dirR = 1;
		}
		if (_g < 0) {
			_g = 0;
			_dirG = 1;
		}
		if (_b < 0) {
			_b = 0;
			_dirB = 1;
		}

		// snakes[i].setFriction(0);

		if (curPixel < TOTAL_PIXEL_COUNT) {
			curPixel += snakeSpacing;
			snakes[i].setActive();
		} else {
			snakes[i].setInactive();
			// curPixel = 0;
		}

	}

}

void freshSnakes() {

	for(int i = 0; i < MAX_SNAKES; i++) {
		snakes[i] = Snake(
			// starting pixel
			(int) random(0,TOTAL_PIXEL_COUNT),
			// length
			(int) random(1, 10),
			// speed
			random(-1000, 1000),
			// RGB:
			(int) random(minR,maxR), (int) random(minG,maxG), (int) random(minB,maxB)
		);

		if (random(0,1) > .9)
			snakes[i].setActive();
	}

}
