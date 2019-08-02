// Interface pins
#define BUTTON_PIN_UP     9
#define BUTTON_PIN_DOWN   10
#define BUTTON_PIN_REMOTE 8
#define POTENTIOMETER_PIN 0
// ^ what is the value of A0 ? 0 ?  maybe ?

// Pixel configuration relative to strip length
#define PIXEL_PIN 6
#define STRIP_PIXEL_LENGTH 300
#define PRACTICAL_PIXEL_COUNT 300
#define MAX_SNAKES 5

// Display modes - eg: all lights off, blinking, cycling, whatever

// Pending more - powered on but not doing anything
#define DISPLAY_MODE_PENDING 0
// Parked mode - minimal lighting, enough to be visible at night
#define DISPLAY_MODE_PARKED  1
// Find: Remote control make the bike flash a lot
#define DISPLAY_MODE_FIND    2
// Snakes!!!!
#define DISPLAY_MODE_SNAKES  3

// Pixel drawing modes (add overlapping dudes or replace)
#define PIXEL_ADDITIVE_MODE    0
#define PIXEL_REPLACEMENT_MODE 1

// Include libraries
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "PixelProxy.h"
#include "Snake.h"
#include "Button.h"

// Current display mode
int displayMode = DISPLAY_MODE_PENDING;

// Current pixel drawing mode
int printMode = PIXEL_ADDITIVE_MODE;

// Animation timing variables
unsigned long previousMillis = 0;
unsigned long cycleMS = 0;
unsigned long elapsedMS = 0;

// Particles, and display
Snake snakes[MAX_SNAKES];
PixelProxy proxy = PixelProxy();
Adafruit_NeoPixel strip(STRIP_PIXEL_LENGTH, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

Button upButton     = Button(BUTTON_PIN_UP);
Button downButton   = Button(BUTTON_PIN_DOWN);
Button remoteButton = Button(BUTTON_PIN_REMOTE);

int potVal = 0;
// int potentiometer = 0; // 0 - 1023

int minR = 100;
int maxR = 255;
int minG = 0;
int maxG = 0;
int minB = 0;
int maxB = 255;

int frameCount = 0;

void setup() {

	Serial.begin(115200);
	Serial.println("Setup starting!");

	// Reserve memory for snakes
	populateSnakes();

	//
	for(int i = 0; i < MAX_SNAKES; i++) {

		snakes[i] = Snake(
			random(0, PRACTICAL_PIXEL_COUNT),
			random(0,50),
			-100 + random(0,200),
			random(0,255), random(0,255), random(0,255)
		);

		snakes[i].setActive();
	}

	strip.begin(); // Initialize NeoPixel strip object (REQUIRED)

	// zero out entire strip
	for(int i = 0; i < STRIP_PIXEL_LENGTH; i++) {
		strip.setPixelColor(i , 0, 0, 0);
	}

	strip.show();  // Initialize all pixels to 'off'

	Serial.println("Setup all set!");

}


void loop() {

	// int potval = analogRead(POTENTIOMETER_PIN);
	// Serial.println(potval);
	// 681 - 1023 // this range do something with it

	/*
	Serial.print( digitalRead(BUTTON_PIN_DOWN) );
	Serial.print(",");
	Serial.print( digitalRead(BUTTON_PIN_UP) );
	Serial.print(",");
	Serial.println( digitalRead(BUTTON_PIN_REMOTE) );
	*/

	elapsedMS = millis() - previousMillis;
	previousMillis = millis();

	readInterface();
	// switch mode here -- eg: fade in / out or solid colors or whatever vs snakemode

	bool hasUpAction = false;
	bool hasDownAction = false;
	bool hasRemoteAction = false;

	if (upButton.hasAction()) {
		upButton.receiveAction();
		hasUpAction = true;
		Serial.println("UP");
		displayMode = DISPLAY_MODE_SNAKES;
		// handle up press here
	}

	if (downButton.hasAction()) {
		downButton.receiveAction();
		hasDownAction = true;
		// handle up press here
		Serial.println("DOWN");
		displayMode = DISPLAY_MODE_PARKED;
	}

	if (remoteButton.hasAction()) {
		remoteButton.receiveAction();
		hasRemoteAction = true;
		elapsedMS = 0;
		if (displayMode == DISPLAY_MODE_SNAKES)
			displayMode = DISPLAY_MODE_PARKED; //
		else
			displayMode = DISPLAY_MODE_SNAKES;
		// handle remote pressed here
		Serial.println("REMOTE");
	}


	// Update strip proxy from visualizer components (eg: parked thing, particle thing, etc)
	switch (displayMode) {

		case DISPLAY_MODE_PENDING:
		default:
			// nothing doing here
			break;

		case DISPLAY_MODE_FIND:
			updateProxyFind(elapsedMS);
			break;

		case DISPLAY_MODE_PARKED:
			updateProxyParked(elapsedMS);
			break;

		case DISPLAY_MODE_SNAKES:
			updateProxyFromSnakes(elapsedMS);
			break;
	}

	// Update strip proxy from visualizations for user interaction (eg: light up some LEDS for button press or what-have-you)
	updateProxyFromIndicator(elapsedMS);

	// Print to LEDs
	updateDisplay();

	cycleMS += elapsedMS;

	if (cycleMS >= 1000) {
		Serial.println(frameCount);
		frameCount = 0;
		cycleMS = 0;
	}

}

void updateProxyFind(unsigned long elapsed) {
	// do some noisy things to show where bike is
}

void updateProxyParked(unsigned long elapsed) {

	// set all default to black
	for(int i = 0; i < PRACTICAL_PIXEL_COUNT; i++) {
		proxy.setPixelColor(i,0,0,0);
	}

	// update from ??

}

void updateProxyFromIndicator(unsigned long elapsed) {

}

void updateProxyBackground(unsigned long elapsed) {

}

void updateProxyFromSnakes(unsigned long elapsed) {

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

			switch( printMode ) {
				case PIXEL_ADDITIVE_MODE:
					proxy.setPixelColorAdditive((startP + j) % PRACTICAL_PIXEL_COUNT, snakes[i].getRAt(j), snakes[i].getGAt(j), snakes[i].getBAt(j));
					break;

				default:
				case PIXEL_REPLACEMENT_MODE:
					proxy.setPixelColor((startP + j) % PRACTICAL_PIXEL_COUNT, snakes[i].getRAt(j), snakes[i].getGAt(j), snakes[i].getBAt(j));
					break;

			}
		}

		snakes[i].update(elapsed, PRACTICAL_PIXEL_COUNT);

	}

}

void updateDisplay() {

	// set all default to black
	for(int i = 0; i < PRACTICAL_PIXEL_COUNT; i++) {
		strip.setPixelColor(i, proxy.getRAt(i), proxy.getGAt(i), proxy.getBAt(i) );
	}

	strip.show();
	frameCount++; // "frame" meaning one single change to the active state of the strip

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

	int _r = random(0,255);
	int _g = random(0,255);
	int _b = random(0,255);
	int _dirR = 1;
	int _dirG = 1;
	int _dirB = 1;

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

		if (curPixel < STRIP_PIXEL_LENGTH) {
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
			(int) random(0,STRIP_PIXEL_LENGTH),
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

void readInterface() {

	upButton.read();
	downButton.read();
	remoteButton.read();
	potVal = analogRead(POTENTIOMETER_PIN);

}
