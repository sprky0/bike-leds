#include <Arduino.h>
#include <Arduino.h>

#define LED_PIN 6
#define LED_COUNT 300

#define MODE_PARKED 0
#define MODE_LIGHT1 1

uint_8t mode = 0;

void setup() {

}

void loop() {

	switch (mode) {
		case MODE_PARKED:
		parkMode();
		break;

	}


}
