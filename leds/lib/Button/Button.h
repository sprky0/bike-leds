#include <Arduino.h>

#ifndef BUTTON_H
#define BUTTON_H

/**
 * Basic class to see if I am going crazy
 */
class Button {

	public:

		Button(uint8_t digitalPin);       // constructor
		void read(); // some member method
		bool hasAction(); // is there an action queued up from the press happening ?
		bool getState(); // get the current pressed state
		uint8_t getPressCount();
		unsigned long getLastPressDuration(); //
		void resetAction(); // say 'ok we got it -- action can be false'

	private:

		uint8_t _pin = 0; // where does our data come from
		bool _hasPressAction = false; // data for "has the thing the button is supposed to do been done" ?
		bool _state = false; // what we have now
		bool _lastState = false; // what we had then
		unsigned long _lastDebounceMillis = 0; // how long do we need to be different to be noticed
		unsigned long _lastPressDuration  = 0; // how long was the last button press?
		unsigned long _lastPressStartMillis  = 0; // how long was the last button press?
		uint8_t _pressCount = 0; // how many discrete presses were there?

		// unsigned long debounceDelayMS = 50;
		// ^ hardcoding this to save a couple bytes

		// @todo track the keypress duration
		// receive the # of presses

};

#endif


// @toto interface cerroncept implementation
/**

- Button has debounce -- how long must it remain changed to count as a real change ?
- Add:
	- Opposite -- of that, after debounce to "off" state -- how long for it to count as "action completed" ?
	- This will allow doubleclick, multiple clicks etc (but not for alternating or combinaton of buttons -- but maybe that's ok)
	- Also store how long the last state was held -- so we can do like, long press vs short press
			- So like, if you do:

				* thing has an action
				* how many clicks ?
					* if 1, was it long press or short ? or whatever
					* if many, do some other thing
				* 'receiveAction' / reset state

	- ya ok that will be cool i am a smart boy
**/
