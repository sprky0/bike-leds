#include <Arduino.h>
#include "Button.h"

#ifndef BUTTON_CPP
#define BUTTON_CPP

#define DEBOUNCE_DELAY_MS 50

/**
 * Button press including debounce
 */
Button::Button(uint8_t pin) {
	_pin = pin;
}

void Button::read() {

	// if actionLastCycle == true
	// claimAction() ? something like that ? to reset

	// Read the state of the switch into a local variable:
	bool reading = digitalRead(_pin);

	// Serial.println( reading );

// If the switch changed, due to noise or pressing:
	if (reading != _lastState) {
		// reset the debouncing timer
		lastDebounceMillis = millis();
	}

	if ((millis() - lastDebounceMillis) > DEBOUNCE_DELAY_MS) {

		// whatever the reading is at, it's been there for longer than the debounce
		// delay, so take it as the actual current state:

		// if the button state has changed:
		if (reading != _state) {

			// this should theoretically happen 1x per debounced change

			_state = reading;

			// Doing this way b/c I don't want to deal with a callback at the moment
			if (_state == true) {
				_hasPressAction = true;
				// iHaveTheBall ->  noIHaveTheBall() <-- to reset ?  something like that
			}

		}

	}

	_lastState = reading;

}

/**
 * Is there button work to do?
 */
bool Button::hasAction() {
	return _hasPressAction;
}

/**
 * Call it!  "I've got this"
 */
void Button::receiveAction() {
	_hasPressAction = false;
}

#endif
