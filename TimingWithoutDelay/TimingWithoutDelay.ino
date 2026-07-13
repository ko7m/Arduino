// TimingWithoutDelay.ino
// Jeff Whitlatch - ko7m
//
// This sample will blink the built-in LED on an Arduino without the use of
// the delay(n) function which will block all other execution.
//

// LED related variables
const int ledPin = LED_BUILTIN;     // The pin number the build-in LED is on
bool ledOn = false;                 // The current state of the LED pin (off)

// Time related variables
const long blinkInterval = 1000L;    // How often to toggle LED on/off (1 second)
unsigned long prevMS = 0L;          // Stores the millisecond counter at the last update of the LED 

// Setup the LED pin as an output
void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // here is where you'd put code that needs to be running all the time.

  // Take a copy of the current millisecond counter so it doesn't change during
  // the calculations that follow.
  unsigned long currMS = millis();

  // A simple subtraction will give you the amount of time that has passed since
  // the last update to the LED.  If greater or equal to the desired interval between
  // blinks, then it is time to toggle the state of the LED
  if (currMS - prevMS >= blinkInterval) {   
    // Save the current time since we are changing the LED state 
    prevMS = currMS;

    // Toggle the LED on or off based on its current state
    ledOn = !ledOn;   
    digitalWrite(ledPin, (int) ledOn);
  }
  // The point of this approach is these tests for the amount of time that has
  // passed can continue to run without blocking other processing.  I can put
  // more code that will run every time through this loop below and the blinking
  // of the LED will continue to function correctly as long as the code below does
  // not require more time that the blinkInterval.  If it does it needs to be broken
  // up into smaller chunks that can be done in less time.
}
