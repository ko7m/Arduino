// Pin change interrupts for Arduino Uno ATMega328
//
// Digital pin     PC Pin     Vector      Ctl Reg Port PCICR Bit Pin Mask Reg
// D0-D7           PCINT16-23 PCINT2_vect PCIR2   PD   PCIE2     PCMSK2
// D8-D13          PCINT0-5   PCINT0_vect PCIR0   PB   PCIE0     PCMSK0
// D14-D19 (A0-A5) PCINT8-13  PCINT1_vect PCIR1   PC   PCIE1     PCMSK1
//

uint32_t count = 0;
volatile byte whathappened; //show which bits changed
volatile boolean writeamus; // flag that interrupt has been changed
volatile byte rx_state; // current state of monitored pins

ISR (PCINT0_vect) {
  byte rx_state_old;
  rx_state_old = rx_state;
  rx_state = PINB;
  whathappened = rx_state ^ rx_state_old;
  writeamus = true;

}

// Timer ISR - We get here if the debounce timer times out.  Call back the encoder, paddle
// and key handlers to process debounced digital inputs.  Disables Timer 1.
ISR(TIMER1_COMPA_vect)
{

}

void setup()
{
  Serial.begin(115200);
  rx_state = PINB;
  Serial.println(rx_state,BIN);
  writeamus = false;
  whathappened = 0;
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  cli();
  PCMSK0 |= 0b00011011;
  PCICR  |= (1<<PCIE0);

  // Set timer 1 for 3 ms debounce timeout
  TCCR1A = 0;             // set entire TCCR1A register to 0
  TCCR1B = 0;             // same for TCCR1B
  TCNT1  = 0;             // initialize counter value to 0

  // set compare match register 3 ms
  OCR1A = 48000;

  // turn on CTC mode
  TCCR1B |= (1 << WGM12);

  // Set bits for no prescaler
  TCCR1B |= (1 << CS10);

  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei();
}

void loop()
{
  cli();
  if (writeamus) {
    //Serial.println("what happenend");
    Serial.println(whathappened,BIN);
    writeamus = false;
  }
  sei();
//  count++;
//  if (count > 500000) {
//    Serial.println("waiting...");
//    count = 0;
//  }

}
