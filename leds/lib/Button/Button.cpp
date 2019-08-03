#include <Arduino.h>
#include "Button.h"

#ifndef BUTTON_CPP
#define BUTTON_CPP

#define DEBOUNCE_DELAY_MS 50
#define ACTION_COMPLETE_DELAY_MS 100

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
		_lastDebounceMillis = millis();
	}

	if ((millis() - _lastDebounceMillis) > DEBOUNCE_DELAY_MS) {

		// whatever the reading is at, it's been there for longer than the debounce
		// delay, so take it as the actual current state:

		// if the button state has changed:
		if (reading != _state) {

			// this should theoretically happen 1x per debounced change

			_state = reading;

			// Doing this way b/c I don't want to deal with a callback at the moment
			if (_state == true) {
				// _hasPressAction = true; 			// iHaveTheBall ->  noIHaveTheBall() <-- to reset ?  something like that
				_lastPressStartMillis = millis();
			} else if (_state == false) {
				_lastPressDuration = millis() - _lastPressStartMillis;
				_pressCount++;
				// wait, we need a duration to this thing somehow
			}

		}

	}

	if (!_hasPressAction && _lastPressStartMillis != 0 && _lastPressDuration != 0 && (millis() - _lastPressStartMillis > ACTION_COMPLETE_DELAY_MS)) {
		// this means we are done with clicky time -- the person has let go and stopped pressing the button for long enough
		// to consider that a completed button pressing session
		_hasPressAction = true;
		Serial.print("PRESS BUTTON TIME IS DONE ");
		Serial.print(_pressCount);
		Serial.print(" presses, last one was ");
		Serial.print(_lastPressDuration);
		Serial.println("MS");
	}

	_lastState = reading;

}

/**
 * Is there button work to do?
 */
bool Button::hasAction() {
	return _hasPressAction;
}

uint8_t Button::getPressCount() {
	return _pressCount;
}

unsigned long Button::getLastPressDuration() {
	return _lastPressDuration;
}

/**
 * We just did the appropriate action or queued action for this, so we reset for next time
 */
void Button::resetAction() {
	_hasPressAction = false;
	_lastPressDuration = 0;
	_pressCount = 0;
}

#endif
