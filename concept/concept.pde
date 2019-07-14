int mode = 0;
int totalPixelCount = 300;

NeoPixelProxy strip;

double previousMillis = 0;
double cycleMillis = 0;

int maxSnakes = 10000;
int snakeCount = 0;

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

	// populateSnakes();
	regularSnakes();

}

void draw() {

	double elapsed = millis() - previousMillis;

	for(int i = 0; i < totalPixelCount; i++) {
		strip.setPixelColor(i,0,0,0);
	}

	for(int i = 0; i < maxSnakes; i++) {

		if (!snakes[i].isActive())
			continue;

		int startP = snakes[i].getPixel();

		for (int j = 0; j < snakes[i].getLength(); j++) {
			strip.setPixelColor((startP + j) % totalPixelCount, snakes[i].getRAt(j), snakes[i].getGAt(j), snakes[i].getBAt(j));
		}

		snakes[i].update(elapsed, totalPixelCount);

	}

	strip.show();

	// switch mode outside this matybe ? diff modes have diff timing i guess
	if (cycleMillis > 250) {

		// addARandomSnake();
		// switch mode etc
		// regularSnakes();
		// freshSnakes();

		// here is something which happens every second
		// eg make one new particle or something like that
		// snakes[snakeCount] = new Snake((int) random(0,300), (int) random(0,255), (int) random(0,255), (int) random(0,255) );
		cycleMillis = 0;
	}

	previousMillis = millis();
	cycleMillis += elapsed;

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

	} else {
		println("I wanted snake but I got " + snakeIndex);
	}

}

void regularSnakes() {

	int curPixel = 0;

	for(int i = 0; i < maxSnakes; i++) {
		println(i);
		snakes[i] = new Snake(
			// starting pixel
			curPixel,
			// length
			5,
			// speed
			500,
			// RGB:
			255,0,0
		);

		snakes[i].setFriction(0.8);

		if (curPixel < totalPixelCount) {
			curPixel += 20;
			snakes[i].setActive();
		} else {
			snakes[i].setInactive();
			curPixel = 0;
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
	float f = 1; // friction

	int [] clr = new int[3];
	int snakeLength = 1; // in pixels
	boolean _protected = false;
	boolean _active = false;

	// what kind of thing is this fucker?  (eg: blinky boy, fader slowly, wwhatever)
	// maybe also include some notion of trailing off at the edges
	int mode = 0;

	Snake(int startPixel, int lengthInPixels, float velocityPixelsPerSecond, int r, int g, int b) {
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
		p *= 1 - ((float) (elapsed / 1000) * f);
		while (p < 0)
			p = 300 + p;
		p = p % pixelCount;
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
		pixels[pixel][0] = r;
		pixels[pixel][1] = g;
		pixels[pixel][2] = b;
	}


}
