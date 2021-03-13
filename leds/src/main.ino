#include "Config.h"

// Include libraries
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "PixelProxy.h"
#include "Snake.h"
#include "Button.h"


// Adafruit_NeoPixel strip(174, 6, NEO_GRB + NEO_KHZ800);
//
// void setup() {
//
// 	strip.begin();
//
// }
//
// int altR = 0;
//
// void loop() {
//
// 	for (uint16_t i = 0; i < 174; i++) {
// 		strip.setPixelColor(i, altR, i, 0 );
// 	}
// 	altR++;
// 	altR = altR % 255;
// 	delay(100);
//
// 	strip.show();
//
// }
//

// Current display mode
int displayMode = DISPLAY_MODE_PENDING;

// Current pixel drawing mode
int printMode = PIXEL_ADDITIVE_MODE;

// Animation timing variables
unsigned long previousMillis = 0;
unsigned long cycleMS = 0;
unsigned long elapsedMS = 0;
unsigned long lastModeChangeMS = 0;

int frameCount = 0;
unsigned long cycleDuration = 1;
int cycleCount = 0;


// Particles, and display
Snake snakes[MAX_SNAKES];
PixelProxy proxy = PixelProxy();
Adafruit_NeoPixel strip(STRIP_PIXEL_LENGTH, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
// Adafruit_NeoPixel strip(174, 6, NEO_GRB + NEO_KHZ800);

Button upButton     = Button(BUTTON_PIN_UP);
Button downButton   = Button(BUTTON_PIN_DOWN);
// Button remoteButton = Button(BUTTON_PIN_REMOTE);

// int lastPpotVal = 0;
int potVal = 0;
float potValFloat = 1.0;


int fR = 160;
int fG = 60;
int fB = 60;

int minR = 100;
int maxR = 255;
int minG = 0;
int maxG = 0;
int minB = 0;
int maxB = 255;

void setup() {

	Serial.begin(115200);
	Serial.println("Setup starting!");

	// Reserve memory for snakes
	populateSnakes();

	// The classic
	getWoodlandSnakes();

	strip.begin(); // Initialize NeoPixel strip object (REQUIRED)

	// zero out entire strip
	for(int i = 0; i < STRIP_PIXEL_LENGTH; i++) {
		strip.setPixelColor(i , 0, 0, 0);
	}

	// strip.setBrightness(255);
	// strip.setBrightness(100);

	strip.show();  // Initialize all pixels to 'off'
	delay(1);

	Serial.println("Setup all set!");

	// showWelcome();

	changeMode( DISPLAY_MODE_SOLID, true );

}


int startIterator = 0;
int endIterator   = 10; // persist outside of loop some shit

// top  R 0-31
// top  L 96-127
// down R 32-63
// down L 64-95
// seat L 128-153
// seat R 154-179
// rear L 200-219
// rear R 180-199  // top to bottom
// front L 240-259 // bottom to top
// front R 220-239 //

void loop() {

	// set all default to black
	proxy.setAll(0,0,0);

	elapsedMS = millis() - previousMillis;
	previousMillis = millis();

	readInterface(elapsedMS);
	// switch mode here -- eg: fade in / out or solid colors or whatever vs snakemode

	bool hasUpAction = false;
	bool hasDownAction = false;
	// bool hasRemoteAction = false;

	if (upButton.hasAction()) {
		hasUpAction = true;
		Serial.println("UP");
		if (upButton.getLastPressDuration() > 1000)
			changeMode(DISPLAY_MODE_PENDING, true);
		else
			changeMode(displayMode + 1);
		upButton.resetAction();
	}

	if (downButton.hasAction()) {
		hasDownAction = true;
		Serial.println("DOWN");
		if (downButton.getLastPressDuration() > 1000)
			changeMode(DISPLAY_MODE_PARKED, true);
		else
			changeMode(displayMode - 1);
		downButton.resetAction();
	}

	// if (remoteButton.hasAction()) {
	// 	hasRemoteAction = true;
	// 	elapsedMS = 0;
	// 	changeMode(DISPLAY_MODE_FIND, true);
	// 	remoteButton.resetAction();
	// }


	// Update strip proxy from visualizer components (eg: parked thing, particle thing, etc)
	switch (displayMode) {

		case DISPLAY_MODE_PENDING:
		default:
			// nothing doing here
			// Serial.println("DOING NOTHING, INTENTIONALLY!");
			break;

		case DISPLAY_MODE_SOLID:
			updateProxySolid( potValFloat );
			break;

		case DISPLAY_MODE_LINES:
			updateProxyLines(elapsedMS);
			break;

		case DISPLAY_MODE_FIRE:
			updateProxyFire(elapsedMS);
			if (potValFloat > 0.5 && random(0,100) > 90) {
				addSnakeSparkles();
			}
			updateProxyFromSnakes(elapsedMS, true);
			break;

		case DISPLAY_MODE_TEST:
			break;

		case DISPLAY_MODE_FIND:
			updateProxyFind(elapsedMS);
			break;

		case DISPLAY_MODE_PARKED:
			updateProxyParked(elapsedMS);
			break;

		case DISPLAY_MODE_SNAKES:
			updateProxyFromSnakes(elapsedMS);
			if (potValFloat > 0.5 && random(0,100) > 90) {
				addARandomSnake();
			}
			break;

		// case DISPLAY_MODE_SNAKEFRICTION:
		// 	updateProxyFromSnakes(elapsedMS);
		// 	addRandomFrictionSnakes(); // snakeFriction
		// 	break;

		case DISPLAY_MODE_FADER:
			updateProxyFader(elapsedMS);
			updateProxyFromSnakes(elapsedMS, true);
			if (potValFloat > 0.5 && random(0,100) > 90) {
				addARandomSnake();
			}
			if (potValFloat > 0.8 && random(0,100) > 90) {
				int snakeIndex = getActiveSnakeIndex();
				if (snakeIndex > 0)
					explodeSnake(snakeIndex);
			}
			break;

		case DISPLAY_MODE_RAINBOW:
			// elapsedMS % 255
			rainbowCycleNonBlock( cycleCount % 255 );
			break;

	}

	// Update strip proxy from visualizations for user interaction (eg: light up some LEDS for button press or what-have-you)
	updateProxyFromIndicator(elapsedMS);

	// Print to LEDs
	updateDisplay();

	cycleMS += elapsedMS;

	if (cycleMS >= cycleDuration) {
		cycleMS = 0;
		cycleCount++;
	}

}

void setCycleDuration(unsigned long ms) {
	cycleCount = 0;
	cycleMS = 0;
	cycleDuration = ms;
}

void changeMode(int targetMode) {
	changeMode( targetMode, false );
}

void changeMode(int targetMode, bool force) {

	if (!force && targetMode < DISPLAY_MODE_MINIMUM) {
		Serial.print("MODE OUT OF RANGE - GOING TO LAST");
		Serial.print( targetMode );
		targetMode = DISPLAY_MODE_MAXIMUM;
	} else if (!force && targetMode > DISPLAY_MODE_MAXIMUM) {
		Serial.print("MODE OUT OF RANGE - GOING TO FIRST");
		Serial.print( targetMode );
		targetMode = DISPLAY_MODE_MINIMUM;
	}

	cycleMS = 0;

	switch (targetMode) {

		default:
		case DISPLAY_MODE_PENDING:
		strip.setBrightness(0);
		break;

		case DISPLAY_MODE_PARKED:
		strip.setBrightness(255);
		populateSnakes(); // clear snakes
		snakes[0] = Snake(
			0,
			50,
			300,
			128, 0, 0
		);
		snakes[0].setMode(1);
		snakes[0].setActive();
		snakes[1] = Snake(
			STRIP_PIXEL_LENGTH / 2,
			50,
			300,
			128, 0, 0
		);
		snakes[1].setMode(1);
		snakes[1].setActive();
		break;

		case DISPLAY_MODE_FIND:
		strip.setBrightness(255);
		populateSnakes();
		for(int i = 0; i < 5; i++) {
			snakes[i] = Snake(
				i * 10,
				5,
				100,
				128, 0, 0
			);
			snakes[i].setActive();
		}
		for(int i = 5; i < 10; i++) {
			snakes[i] = Snake(
				i * 10,
				5,
				1000,
				255, 255, 255
			);
			snakes[i].setActive();
		}
		break;

		case DISPLAY_MODE_SNAKES:
		strip.setBrightness(255);
		getWoodlandSnakes();
		break;

		// case DISPLAY_MODE_SNAKEFRICTION:
		// strip.setBrightness(255);
		// populateSnakes();
		// break;

		case DISPLAY_MODE_FADER:
		fR = random(10,160);
		fG = random(10,160);
		fB = random(10,160);
		populateSnakes();
		strip.setBrightness(255);
		break;

		case DISPLAY_MODE_FIRE:
		strip.setBrightness(255);
		populateSnakes(); // clear me
		break;

		case DISPLAY_MODE_LINES:
		strip.setBrightness(255);
		break;

		case DISPLAY_MODE_RAINBOW:
		cycleCount = 0;
		cycleMS = 1;
		break;

		case DISPLAY_MODE_SOLID:
		cycleCount = 0;
		cycleMS = 0;
		break;

	}


	displayMode = targetMode;
	lastModeChangeMS = millis();

	Serial.print("We changed mode to ");
	Serial.println( displayMode );

}

void updateProxyFind(unsigned long elapsed) {
	// do some noisy things to show where bike is
	updateProxyFromSnakes(elapsed, true);

	if (millis() - lastModeChangeMS > DISPLAY_FIND_TIMEOUT) {
		changeMode(DISPLAY_MODE_PARKED, true);
	}

}

void updateProxyParked(unsigned long elapsed) {
	updateProxyFromSnakes(elapsed, true);
}

void updateProxyFader(unsigned long elapsed) {

	// fade up
	float fade = (float) (cycleCount % 40) / 40;

	// scale to 0-2
	fade = (fade * 2);
	// second half scale fade down
	if (fade > 1) {
		fade = 2 - fade;
	}

	proxy.setAll(fade * fR, fade * fG, fade * fB);

}

void updateProxyLines(unsigned long elapsed) {

		proxy.setAll(50,50,0);

		int offset = cycleCount % STRIP_PIXEL_LENGTH;

		for (int i = offset; i < offset + 80; i++) {
			proxy.setPixelColor(i % STRIP_PIXEL_LENGTH, 50, 0, 0);
		}

		offset = (cycleCount % STRIP_PIXEL_LENGTH) + 80;

		for (int i = offset; i < offset + 160; i++) {
			proxy.setPixelColor(i % STRIP_PIXEL_LENGTH, 0, 0, 50);
		}

}

void updateProxyFromIndicator(unsigned long elapsed) {

	if (upButton.getState()) {
		for(int i = 21; i <= 31; i++) {
			proxy.setPixelColor(i,255,127,0);
		}
		for(int i = 96; i <= 106; i++) {
			proxy.setPixelColor(i,255,127,0);
		}
	}

	if (downButton.getState()) {
		for(int i = 21; i <= 31; i++) {
			proxy.setPixelColor(i,255,0,0);
		}
		for(int i = 96; i <= 106; i++) {
			proxy.setPixelColor(i,255,0,0);
		}
	}

	// if (remoteButton.getState()) {
	// 	for(int i = 21; i <= 31; i++) {
	// 		proxy.setPixelColor(i,0,0,255);
	// 	}
	// 	for(int i = 96; i <= 106; i++) {
	// 		proxy.setPixelColor(i,0,0,255);
	// 	}
	// }

}

void updateProxyBackground(unsigned long elapsed) {

}

void updateProxyFromSnakes(unsigned long elapsed, bool noclear) {

	if (noclear == false) {
		// set all default to black
		proxy.setAll(0,0,0);
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

void updateProxyFromSnakes(unsigned long elapsed) {

	updateProxyFromSnakes(elapsed, false);

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

		if (random(0,10) > 7)
			snakes[i].setActive();
	}

}

void readInterface(unsigned long elapsedMS) {

	upButton.read();
	downButton.read();
	// remoteButton.read();

	int _potVal = analogRead(A0);

	if (abs(potVal - _potVal) > POTENTIOMETER_SLEW_LIMIT) {
		if (potVal - _potVal > 0)
			potVal -= POTENTIOMETER_SLEW_LIMIT;
		else
			potVal += POTENTIOMETER_SLEW_LIMIT;
	}

	/*
	if (potVal < 680)
		potVal = 680;
	else if (potVal > 1023)
		potVal = 1023;

	// potVal = (potVal + _potVal) / 2;

	// 680 - 1023 RANGE FOR SOME REASON
	// potValFloat = (float) (potVal - 680) / 343; // << values for the bike
	*/
	potValFloat = (float) potVal / 1024; // value for 10k resistor M5 version

	if (potValFloat < 0) {
		potValFloat = 0;
	}

	// Serial.println(potValFloat	);

}

void setAllLEDs(int r, int g, int b) {
	for(int i = 0; i < STRIP_PIXEL_LENGTH; i++) {
		strip.setPixelColor(i, r,g,b);
	}
	strip.show();
}

void showWelcome() {

	// for (uint16_t r = 0; r < 255; r++) {
	// 	for (uint16_t g = 0; g < 255; g++) {
	// 		for (uint16_t b = 0; b < 255; b++) {
	// 			setAllLEDs(r,g,b);
	// 		}
	// 		// delay(1);
	// 		strip.show();
	// 	}
	// }

	for (uint16_t r = 0; r < 255; r++) {
		uint32_t c = getColorWheelValue(r);
		setAllLEDs(
			getRed(c),
			getGreen(c),
			getBlue(c)
		);
		delay(1);
	}
	// for (uint16_t i = 0; i < STRIP_PIXEL_LENGTH; i++) {
	// 	strip.setPixelColor(i,0,0,0);
	// }
	// strip.show();


	delay(1000);

	// for(int i = 0; i < 50; i++) {
	// 	setAllLEDs(i, i / 2,i / 3);
	// 	delay(1);
	// }
	// for(int i = 50; i > 0; i--) {
	// 	setAllLEDs(i, i / 2,i / 3);
	// 	delay(1);
	// }
	//
	setAllLEDs(0,0,0);

}

// void updateProxyCycler()

void updateProxyFire(unsigned long elapsedMS) {

	// Serial.println(cycleCount);

	// int cur = 0;
	int s = 0;
	int e = 0;

	proxy.setAll(random(25,potValFloat * 50),random(0,potValFloat * 25),0);

	switch (cycleCount % 10) {

		// top  R 0-31
		case 0: s = 0; e = 31; break;
		// top  L 96-127
		case 1: s = 96; e = 127; break;
		// down R 32-63
		case 2: s = 32; e = 63; break;
		// down L 64-95
		case 3: s = 64; e = 95; break;
		// seat L 128-153
		case 4: s = 128; e = 153; break;
		// seat R 154-179
		case 5: s = 154; e = 179; break;
		// rear L 200-219
		case 6: s = 200; e = 219; break;
		// rear R 180-199  // top to bottom
		case 7: s = 180; e = 199; break;
		// front L 240-259 // bottom to top
		case 8: s = 240; e = 259; break;
		// front R 220-239 //
		case 9: s = 220; e = 239; break;

	}

	// burst level = knob
	for(int i = s; i <= e; i++) {
		proxy.setPixelColor( i, potValFloat * 200 + 55, potValFloat * 225 + 24, 0 );
	}

}

void updateProxySolid(float val) {
	// proxy.setPixelColor( potValFloat * 255);
	byte gral = 255 * val;

	Serial.print(val);
	Serial.print(" ");
	Serial.print(gral);
	Serial.println();

	uint32_t kolor = getColorWheelValue( gral );
	proxy.setAll(
		getRed(kolor),
		getGreen(kolor),
		getBlue(kolor)
	);

}

void getWoodlandSnakes() {

	for(int i = 0; i < MAX_SNAKES; i++) {

		snakes[i] = Snake(
			random(0, PRACTICAL_PIXEL_COUNT),
			// random(0,50),
			random(0,5),
			-100 + random(0,200),
			// random(0,255), random(0,255), random(0,255)
			random(0,50), random(0,255), random(0,50)
		);

		snakes[i].setActive();

	}

}

void addSnakeSparkles() {
	// bright white and not many of them

	for(int i = 0; i < random(4,8); i++) {
		int snakeIndex = getFreeSnakeIndex();
		snakes[snakeIndex] = Snake(
			random(0, STRIP_PIXEL_LENGTH),
			1,
			-100 + random(0,200),
			255,255,255
		);
		snakes[snakeIndex].setLifetime(500);
		snakes[snakeIndex].setActive();
	}

}

// void addRandomFrictionSnakes() {
//
// 	if (random(1,100) > 95) {
//
// 		int snakeIndex = getFreeSnakeIndex();
//
// 		snakes[snakeIndex] = Snake(
// 			random(0, PRACTICAL_PIXEL_COUNT),
// 			// random(0,50),
// 			random(5,20),
// 			-200 + random(0,400),
// 			// random(0,255), random(0,255), random(0,255)
// 			random(0,255), random(0,155), random(0,50)
// 		);
//
// 		snakes[snakeIndex].setFriction(.6);
// 		// snakes[snakeIndex].setLoopDeath(true); // loop death is really weird with tails lets do fade instead
// 		snakes[snakeIndex].setLifetime(5000);
// 		snakes[snakeIndex].setActive();
//
// 	}
//
//
// }

uint8_t getRed(uint32_t color) {
	return (color >> 16) & 0xFF;
}

// Returns the Green component of a 32-bit color
uint8_t getGreen(uint32_t color) {
	return (color >> 8) & 0xFF;
}

// Returns the Blue component of a 32-bit color
uint8_t getBlue(uint32_t color) {
	return color & 0xFF;
}






// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycleNonBlock(uint16_t j) {
	// 0 - 255 for outer wheelness
	uint16_t i;
	for(i=0; i < STRIP_PIXEL_LENGTH; i++) {
		uint32_t color = getColorWheelValue(((i * 256 / STRIP_PIXEL_LENGTH) + j) & 255);
		proxy.setPixelColor(i, getRed(color), getGreen(color), getBlue(color));
	}
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t getColorWheelValue(byte WheelPos) {
	WheelPos = 255 - WheelPos;
	if(WheelPos < 85) {
		return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
	}
	if(WheelPos < 170) {
		WheelPos -= 85;
		return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
	}
	WheelPos -= 170;
	return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
