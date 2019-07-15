// int MODE_SNAKE = // etc something like this - which mode are we switched into, parking, blinky boy etc, generative wahtever
static int MODE_XMAS = 1225;
int mode = 0;

int totalPixelCount = 300;
int practicalPixelCount = 300;

NeoPixelProxy strip;

double previousMillis = 0;
double cycleMillis = 0;


int maxSnakes = 300; // we don't really need more than we have pixels right ?
// int snakeCount = 0; //

int minR = 100;
int maxR = 255;
int minG = 0;
int maxG = 0;
int minB = 0;
int maxB = 255;

Snake [] snakes = new Snake[maxSnakes];

void setup() {

	size(1200, 40);

	strip = new NeoPixelProxy(totalPixelCount);

	// alwaysdo this at the beginning to get a bunch of blank snake memory
	populateSnakes();
	// regularSnakes();
	mode = MODE_XMAS;

}

void draw() {

	double elapsed = millis() - previousMillis;

	updateDisplay(elapsed);

	switch(mode) {

		case 1225:
			if (cycleMillis > 500) {
				// explodeSnake();
				int snakeIndex = getFreeSnakeIndex();
				snakes[snakeIndex] = new Snake(
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
				snakes[snakeIndex] = new Snake(
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
				snakes[snakeIndex] = new Snake(
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

		default:

			// switch mode outside this matybe ? diff modes have diff timing i guess
			if (cycleMillis > 250) {

				addARandomSnake();

				if (random(0,1) > 0.5) {
					println("Gonna splode a fool");
					int sploder = getActiveSnakeIndex();
					if (sploder > 0)
						explodeSnake(sploder);
					else
						println("Couldn't find a fool to splode");
				}

				cycleMillis = 0;
			}

		break;

	}

	previousMillis = millis();
	cycleMillis += elapsed;

}

void updateDisplay(double elapsed) {

	// set all default to black
	for(int i = 0; i < practicalPixelCount; i++) {
		strip.setPixelColor(i,0,0,0);
	}

	// set all to pixel color from
	for(int i = 0; i < maxSnakes; i++) {

		if (!snakes[i].isActive())
			continue;

		int startP = snakes[i].getPixel();

		for (int j = 0; j < snakes[i].getLength(); j++) {
			// strip.setPixelColor((startP + j) % practicalPixelCount, snakes[i].getRAt(j), snakes[i].getGAt(j), snakes[i].getBAt(j));
			strip.setPixelColorAdditive((startP + j) % practicalPixelCount, snakes[i].getRAt(j), snakes[i].getGAt(j), snakes[i].getBAt(j));
		}

		snakes[i].update(elapsed, practicalPixelCount);

	}

	strip.show();

}

int getActiveSnakeIndex() {
	// try 10x to get a random nonprotected snake
	int tries = 0;
	while (tries < maxSnakes) {
		int snakeTry = (int) random(0,maxSnakes);
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
	for(int i = 0; i < maxSnakes; i++) {
		if (!snakes[i].isActive()) {
			return i;
		}
	}

	// try 10x to get a random nonprotected snake
	int tries = 0;
	while (tries < 10) {
		int snakeTry = (int) random(0,maxSnakes);
		if (!snakes[snakeTry].isProtected()) {
			return snakeTry;
		}
		tries ++;
	}
	// we totally fail:
	return -1;
}

void populateSnakes() {
	for(int i = 0; i < maxSnakes; i++) {
		snakes[i] = new Snake(0, 1, 0, 0, 0, 0);
	}
}

void explodeSnake(int snakeIndex) {

	int startPixel = snakes[snakeIndex].getPixel();
	int endPixel = snakes[snakeIndex].getLength();
	float velocityBase = snakes[snakeIndex].getVelocity();

	for(int i = 0; i < 6; i++) {
		int snakeIndexTemporary = getFreeSnakeIndex();
		if (snakeIndexTemporary > 0) {
			snakes[snakeIndexTemporary] = new Snake(
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

		println("GOT" + snakeIndex);

		snakes[snakeIndex] = new Snake(
			0,// starting pixel
			(int) random(0,10),// length
			// speed
			random(-100,100),
			// RGB:
			(int)random(0,255),(int)random(0,255),(int)random(0,255)
		);
		snakes[snakeIndex].setActive();
		snakes[snakeIndex].setLifetime( (int) random(1000,5000) );

	} else {
		println("I wanted snake but I got " + snakeIndex);
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

	println(snakeLength + " vs " + snakeSpacing);

	for(int i = 0; i < maxSnakes; i++) {

		// println(i);
		snakes[i] = new Snake(
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

		if (curPixel < totalPixelCount) {
			curPixel += snakeSpacing;
			snakes[i].setActive();
			println("Added " + i + " " + curPixel + " " + totalPixelCount);
		} else {
			snakes[i].setInactive();
			// curPixel = 0;
		}

	}

}

void freshSnakes() {

	for(int i = 0; i < maxSnakes; i++) {
		println(i);
		snakes[i] = new Snake(
			// starting pixel
			(int) random(0,totalPixelCount),
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


class Snake {

	float p = 0; // you can be between 2 pixels but getPixel will deal with it
	float v = 0; // velocity
	float f = 0; // friction

	double bornMillis = -1;
	double lifetimeMS = -1;
	int [] clr = new int[3];
	int snakeLength = 1; // in pixels
	boolean _protected = false;
	boolean _active = false;

	// what kind of thing is this fucker?  (eg: blinky boy, fader slowly, wwhatever)
	// maybe also include some notion of trailing off at the edges
	int mode = 0;

	Snake(int startPixel, int lengthInPixels, float velocityPixelsPerSecond, int r, int g, int b) {

		bornMillis = millis();

		// what pixel are we 'on'
		setPixel(startPixel);
		setVelocity(velocityPixelsPerSecond);
		snakeLength = lengthInPixels;
		clr[0] = r;
		clr[1] = g;
		clr[2] = b;
	}

	void setPixel(int pixel) {
		p = pixel;
	}

	void setVelocity(float velocityPixelsPerSecond) {
		v = velocityPixelsPerSecond;
	}

	void setFriction(float frictionPerSecond) {
		f = frictionPerSecond;
	}

	void setLifetime(double lifetime) {
		lifetimeMS = lifetime;
	}

	void setActive() {
		_active = true;
	}

	void setInactive() {
		_active = false;
	}

	void setProtected() {
		_protected = true;
	}

	void setUnprotected() {
		_protected = false;
	}

	// passing elapsed time so we don't need to internally calculate it on each snake
	void update(double elapsed, int pixelCount) {
		p = (p + ((float) (elapsed / 1000) * v));
		// p *= 1 - ((float) (elapsed / 1000) * f);
		v *= 1 - ((float) (elapsed / 1000) * f);

		while (p < 0)
			p = pixelCount + p;

		p = p % pixelCount;

		if (lifetimeMS > 0 && millis() - bornMillis > lifetimeMS) {
			setInactive();
			setPixel(0);
		}

	}

	float getVelocity() {
		return v;
	}

	int getLength() {
		return snakeLength;
	}

	int getPixel() {
		return (int) p;
	}

	// pos = 0 to (length-1)
	int getRAt(int pos) {
		return clr[0];
	}

	int getGAt(int pos) {
		return clr[1];
	}

	int getBAt(int pos) {
		return clr[2];
	}

	boolean isProtected() {
		return _protected;
	}

	boolean isActive() {
		return _active;
	}

}

/**
 * Class that simulates approximately what NeoPixel lib has for its API
 * Also draws the thing w/ .show()
 */
class NeoPixelProxy {

	int brightness = 255;
	int [][] pixels;
	int pixelCount = 0;

	// there's also pin and some other crap in the constructor for NeoPixel library
	NeoPixelProxy(int num) {
		pixelCount = num;
		pixels = new int[pixelCount][3];
	}

	void setBrightness(int b) {
		// if (b < 0 || b > 255)
		// 	throw(new Exception("Brightness out of range"));
		brightness = b;
	}

	void show() {
		int w = width / pixelCount;
		int h = height;
		for(int i = 0; i < pixelCount; i++) {
			// pixels[i]
			noStroke();
			fill(pixels[i][0],pixels[i][1],pixels[i][2]);
			rect(i * w, 0, w, h);
		}
	}

	void setPixelColor(int pixel, int r, int g, int b) {
		if (pixel >= 0 && pixel < totalPixelCount) {
			pixels[pixel][0] = r;
			pixels[pixel][1] = g;
			pixels[pixel][2] = b;
		}
	}

	void setPixelColorAdditive(int pixel, int r, int g, int b) {

		if (pixel >= 0 && pixel < totalPixelCount) {

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

}
