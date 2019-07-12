int mode = 0;
int totalPixelCount = 300;

NeoPixelProxy strip;

double previousMillis = 0;
double cycleMillis = 0;

int maxSnakes = 300;
int snakeCount = 0;

int minR = 0;
int maxR = 255;
int minG = 0;
int maxG = 0;
int minB = 0;
int maxB = 0;

Snake [] snakes = new Snake[maxSnakes];

void setup() {

	size(1200, 40);
	strip = new NeoPixelProxy(totalPixelCount);

	populateSnakes();

}

void draw() {

	double elapsed = millis() - previousMillis;

	for(int i = 0; i < totalPixelCount; i++) {
		strip.setPixelColor(i,0,0,0);
	}

	for(int i = 0; i < maxSnakes; i++) {

		if (!snakes[i].isActive())
			continue;

		// print("working on snake " + i + " - ");

		int startP = snakes[i].getPixel();

		for (int j = 0; j < snakes[i].getLength(); j++) {
			// print( " " + snakes[i].getLength() + j + " to " + snakes[i].getRAt(j) + "," + snakes[i].getGAt(j) + "," + snakes[i].getBAt(j) );
			strip.setPixelColor((startP + j) % totalPixelCount, snakes[i].getRAt(j), snakes[i].getGAt(j), snakes[i].getBAt(j));
		}

		// println("");

		snakes[i].update(elapsed, totalPixelCount);

	}

	strip.show();

	if (cycleMillis > 1000) {

		freshSnakes();
		// here is something which happens every second
		// eg make one new particle or something like that
		// snakes[snakeCount] = new Snake((int) random(0,300), (int) random(0,255), (int) random(0,255), (int) random(0,255) );
		cycleMillis = 0;
	}

	previousMillis = millis();
	cycleMillis += elapsed;

}

void populateSnakes() {
	for(int i = 0; i < maxSnakes; i++) {
		snakes[i] = new Snake(0, 1, 0, 0, 0, 0);
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

	int [] clr = new int[3];
	int snakeLength = 1; // in pixels
	boolean active = false;

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

	void setActive() {
		active = true;
	}

	void setInactive() {
		active = false;
	}

	// passing elapsed time so we don't need to internally calculate it on each snake
	void update(double elapsed, int pixelCount) {
		p = (p + ((float) (elapsed / 1000) * v));
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

	boolean isActive() {
		return active;
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
