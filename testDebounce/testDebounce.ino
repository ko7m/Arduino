// testDebounce.ino - Implements a 3 ms debounce timer on interrupt pin 2
//
// Jeff Whitlatch - ko7m
//
// Whenever interruptPin changes state it will fire an interrupt.  The interrupt
// handler will reset a 3 ms hardware timer every time the pin changes state.
// Once it has been stable for 3 ms, the timer will interrupt indicating that the
// pin has stopped changing and so the current value of the pin is updated at that
// point in time.
//

const byte ledPin = 13;
const byte interruptPin = 2;
volatile byte state = LOW;
static byte last_value;

void setup() {
    cli();

    pinMode(ledPin, OUTPUT);
    pinMode(interruptPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(interruptPin), pinChange, CHANGE);
    
    // Set timer 1 for 3 ms debounce timeout
    //
    TCCR1A = 0; // set entire TCCR1A register to 0
    TCCR1B = 0; // same for TCCR1B
    TCNT1  = 0; //initialize counter value to 0
    
    // set compare match register 3 ms
    OCR1A = 48000;
    
    // turn on CTC mode
    TCCR1B |= (1 << WGM12);
    
    // Set bits for no prescaler
    TCCR1B |= (1 << CS10); 
    
    // enable timer compare interrupt
    TIMSK1 |= (1 << OCIE1A);
    sei();

    last_value = getPinValue();
}

void loop() { }

// When the pin changes, compare it to the last value read and if it changed,
// just reset the debounce timer.
//
void pinChange()
{
    static byte last;
    byte prior;

    prior = last;
    last = digitalRead(interruptPin);

    // Reset timer1 to zero
    //
    if (prior != last)
    {
        TCNT1 = 0;
        TIMSK1 |= (1 << OCIE1A);
    }
}

// Get pin value
//
static inline byte getPinValue(void)
{
    return digitalRead(interruptPin);

}

// Debounce timer ISR.  We get hewre if the debounce timer times out.  Timer and
// interrupt are left disabled.
//
ISR(TIMER1_COMPA_vect)
{
  byte new_value = getPinValue();
  
  if (new_value != last_value)
  {
    last_value = new_value;
  }
  
  TIMSK1 &= ~(1 << OCIE1A);  // Disable Timer
}
