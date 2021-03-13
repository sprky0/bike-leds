// Interface pins
#define BUTTON_PIN_UP     9  // 10
#define BUTTON_PIN_DOWN   10 // 9
#define BUTTON_PIN_REMOTE 8
#define POTENTIOMETER_PIN 0
// ^ what is the value of A0 ? 0 ?  maybe ?

#define POTENTIOMETER_SLEW_LIMIT 3

// Pixel configuration relative to strip length
#define PIXEL_PIN 6
#define STRIP_PIXEL_LENGTH 174
#define PRACTICAL_PIXEL_COUNT 174
#define MAX_SNAKES 50

// Display modes - eg: all lights off, blinking, cycling, whatever

// #define DISPLAY_MODE_SNAKES
// Pending more - powered on but not doing anything
#define DISPLAY_MODE_MINIMUM 1  // The lowest possible mode level
#define DISPLAY_MODE_SOLID   1
#define DISPLAY_MODE_SNAKES  -1
#define DISPLAY_MODE_RAINBOW 2
#define DISPLAY_MODE_FADER   3
#define DISPLAY_MODE_FIRE    4
#define DISPLAY_MODE_LINES   5
// #define DISPLAY_MODE_RAINBOW 6
#define DISPLAY_MODE_MAXIMUM 2

#define DISPLAY_MODE_PENDING 100  // Waiting for stuff to happen -- don't do anything yet
#define DISPLAY_MODE_PARKED  101  // Parked mode - minimal lighting, enough to be visible at night
#define DISPLAY_MODE_FIND    102  // Find: Remote control make the bike flash a lot
// Snakes!!!!

#define DISPLAY_MODE_TEST    200


// How long to go back to parked after 'find' ?
#define DISPLAY_FIND_TIMEOUT 10000

// Pixel drawing modes (add overlapping dudes or replace)
#define PIXEL_ADDITIVE_MODE    0
#define PIXEL_REPLACEMENT_MODE 1
