int mode = 0;
int count = 300;

NeoPixelProxy strip;

double previousMillis = 0;
double cycleMillis = 0;

Snake [] snakes;

void setup() {
  size(1200, 4);
	strip = new NeoPixelProxy(count);
}

void draw() {

	double elapsed = millis() - previousMillis;

	println(elapsed);

	for (int i = 0; i < count; i++) {
		// strip.setPixelColor(i, (int) random(0,255), (int) random(0,255), (int) random(0,255));
	}
	// strip.show();


	if (cycleMillis > 1000) {
		snakes.push( new Snake( (int) random(5,10), (int) random(0,255), (int) random(0,255), (int) random(0,255) ) );
		cycleMillis = 0;
	}

	previousMillis = millis();
	cycleMillis += elapsed;

}



class Snake {

	int [] clr = new int[3];
	int length = 4;
	boolean active = true;

	Snake(int velocityPixelsPerSecond, int r, int g, int b) {
		p = 0;
		v = velocityPixelsPerSecond;
		clr[0] = r;
		clr[1] = g;
		clr[2] = b;
	}

	// passing elapsed time so we don't need to internally calculate it on each particle
	void update(double elapsed) {
		p = p + (int) ((elapsed / 1000) * velocityPixelsPerSecond);
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

	NeoPixelProxy(int num) {
		pixelCount = num;
		pixels = new int[pixelCount][3];
	}

	// int

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
