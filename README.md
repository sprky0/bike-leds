# Desert Bike

^ Little dumb homage to Desert Bus.

## LED visualizer program

Accepts input via 3 switches and a potentiometer.  Outputs to a strip of Neopixel '2812' LEDS.  First run of the bike is @ 286 or something?  Basically enough to cover the fork both sides, top/seat/down tubes both sides, and the seat stay both sides.  Skipped the chainstay b/c I was using a coaster brake for this bike and it needs a little clamp thing on one side to hold the brake arm.  Still looks good, sort of matches the fork angle in the front.

## Materials

* A bike
* Lots of white Gaffer's tape to attach LEDs to bike
  * The LEDs shine through Gaff nicely, but you may attach them any way you like
* Arduino Mega
* 2 momentary buttons for program up/down control
* 1 more momentary button or remote control circuit for 'find' state control
* 5000k Potentiometer
* 220ohm resistor to prevent backsplash on LED data pin
* 10k ohm resistors for load after buttons (see Arduino docs on wiring buttons)
* Optional 10k ohm resistors after the pot the really juice up that puppy (see Arduino docs on wiring potentiometers)
* Project case to hold ^ this crap
* Something to hold the battery, seat bag, bike bag, whatever
* ~2A many mAh battery.  I used an Anker 26800 big boy
  * You can still overload this batt's output and case it to shut down, so careful with bright white LED levels
  * Bright white sparkles etc are ok but not the full strip (it's more amps than the batt can safely push and the current surge makes it shut down, best case)

## Interface

* **Up** button - Short press: program up, Long press: jump to pending / silent mode
* **Down** button - Short press: program change down, Long press: jump to parked mode
* **Remote** switch on/off 1x - "Find my Bike" mode, which then goes back to parking mode
* **Potentiometer - I implemented this as 'intensity' knob -- animations get crazyier towards the right side, more chill to the left side.

## Getting Started

This program uses a couple of libraries and C++ classes.  Need to make sure you have platforio installed, and can push builds via that.

* Install platformio
* Install atom.io
  * add Platomformio package to atom
* Open leds/main.ino to edit or for builds etc.
* From, atom, to:
  * Build press `COMMAND`-`SHIFT`-`B`
  * Clean the environment press `COMMAND`-`SHIFT`-`C`
  * Upload to the board, press `COMMAND`-`SHIFT`-`U`
* From terminal to monitor the Serial output, run `platformio device monitor --baud 115200`
