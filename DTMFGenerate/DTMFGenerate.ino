// DTMF wave generator using DDS techniques
// Jeff Whitlatch - ko7m
//

#include <avr/pgmspace.h>

#define PWM_PIN          10
#define TONE_COUNT        7
#define HALF_VCC        128

// Single period sine wave table.
// Phase points are 0-255 with 128 as zero crossing.  256 samples per period.
// We store the entire table though in practice it is only necessary to store
// 1/4 of the table since each quarter cycle is symetrical.
//
PROGMEM const unsigned char sine256[] = 
{
    128,131,134,137,140,143,146,149,152,155,158,162,165,167,170,173,
    176,179,182,185,188,190,193,196,198,201,203,206,208,211,213,215,
    218,220,222,224,226,228,230,232,234,235,237,238,240,241,243,244,
    245,246,248,249,250,250,251,252,253,253,254,254,254,255,255,255,
    255,255,255,255,254,254,254,253,253,252,251,250,250,249,248,246,
    245,244,243,241,240,238,237,235,234,232,230,228,226,224,222,220,
    218,215,213,211,208,206,203,201,198,196,193,190,188,185,182,179,
    176,173,170,167,165,162,158,155,152,149,146,143,140,137,134,131,
    127,124,121,118,115,112,109,106,103,100, 97, 93, 90, 88, 85, 82,
     79, 76, 73, 70, 67, 65, 62, 59, 57, 54, 52, 49, 47, 44, 42, 40,
     37, 35, 33, 31, 29, 27, 25, 23, 21, 20, 18, 17, 15, 14, 12, 11,
     10,  9,  7,  6,  5,  5,  4,  3,  2,  2,  1,  1,  1,  0,  0,  0,
      0,  0,  0,  0,  1,  1,  1,  2,  2,  3,  4,  5,  5,  6,  7,  9,
     10, 11, 12, 14, 15, 17, 18, 20, 21, 23, 25, 27, 29, 31, 33, 35,
     37, 40, 42, 44, 47, 49, 52, 54, 57, 59, 62, 65, 67, 70, 73, 76,
     79, 82, 85, 88, 90, 93, 97,100,103,106,109,112,115,118,121,124
};

// Useful macros for setting and resetting bits
//
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |=  _BV(bit))

// DDS frequencies for each DTMF tone and reference clock frequency
//
// DTMF Tones are assigned as follows:
//
//  1209Hz 1336Hz 1477Hz
//    1      2      3     - 697Hz
//    4      5      6     - 770Hz
//    7      8      9     - 852Hz
//    *      0      #     - 941Hz
//
double rgdds_frequency[7] = {697.0, 770.0, 852.0, 941.0, 1209.0, 1336.0, 1477.0}; 
const double ref_frequency = (16000000/500);  // 32 kHz

// Index to dds frequencies for both DTMF tones
//
int tone_low  = 0;
int tone_high = 4;

// These must all be marked as volatile as they are used in an interrupt service routine
// and we don't want the compiler optimizer messing with them.
//
// We are generating from a list of 7 frequencies, two at a time.
//
volatile byte     sine_table_index;
volatile uint32_t phase_accumulator_low;
volatile uint32_t phase_accumulator_high;
volatile uint32_t rgtuning_word[7];         // 7 DTMF tones

// Set things up, Arduino style
//
void setup()
{
    Serial.begin(115200);
    
    // PWM output for timer2 is pin 10 on the ATMega2560
    // If you use an ATMega328 (such as the UNO) you need to make this pin 11
    //
    pinMode(PWM_PIN, OUTPUT);   // Timer 2 PWM output on mega256 is pin 10
    
    // Set up timer2 to a phase correct 32kHz clock
    //
    timer2Setup();
    
    // Calculate the DDS tuning words for all DTMF tone frequencies
    //
    for (int i = 0; i < TONE_COUNT; i++)
      rgtuning_word[i] = pow(2,32) * rgdds_frequency[i] / ref_frequency;
}

// Enable or disable tone generating.
//
void enableTones(bool fEnable)
{
    if (fEnable)
        sbi (TIMSK2,TOIE2);    // Enable timer 2. 
    else
    {
        cbi (TIMSK2,TOIE2);    // Disable timer 2. 
        OCR2A = HALF_VCC;      // Set output to 1/2 VCC
    }
}

// Convert character version of digit to a pair of tones
//
void setTonesFromNumber(char ch)
{
    switch (ch)
    {
        case '1': tone_low = 0; tone_high = 4; break;
        case '2': tone_low = 0; tone_high = 5; break;
        case '3': tone_low = 0; tone_high = 6; break;
        case '4': tone_low = 1; tone_high = 4; break;
        case '5': tone_low = 1; tone_high = 5; break;
        case '6': tone_low = 1; tone_high = 6; break;
        case '7': tone_low = 2; tone_high = 4; break;
        case '8': tone_low = 2; tone_high = 5; break;
        case '9': tone_low = 2; tone_high = 6; break;
        case '*': tone_low = 3; tone_high = 4; break;
        case '0': tone_low = 3; tone_high = 5; break;
        case '#': tone_low = 3; tone_high = 6; break;
        default : tone_low = 0; tone_high = 0; break;
    }
}

// Dial a test phone number
//
void loop()
{
    char *phone = "+1 425-555-1212";
    char *pch = phone;
    while (*pch)
    {
        if (*pch == '-' || *pch == ' ' || *pch == '+')
            if (!*pch++)
                return;
          
        setTonesFromNumber(*pch++);
        enableTones(true); 
        delay(50);
        enableTones(false);
        delay(50);
 
    }
    Serial.println();
    delay(500);
    while(1);
}

// Setup timer2 with prescaler = 1, PWM mode to phase correct PWM
// See the ATMega2560 datasheet for all the gory details
//
void timer2Setup()
{
    // Clock prescaler = 1
    sbi (TCCR2B, CS20);      // 001 = no prescaling
    cbi (TCCR2B, CS21);
    cbi (TCCR2B, CS22);

    // Phase Correct PWM
    cbi (TCCR2A, COM2A0);    // 10 = clear OC2A compare match
    sbi (TCCR2A, COM2A1);

    // Mode 1
    sbi (TCCR2A, WGM20);     // See table 20-8 in datasheet
    cbi (TCCR2A, WGM21);
    cbi (TCCR2B, WGM22);
}

// Timer 2 interrupt service routine (ISR) is used to generate
// the timebase reference clock for the DDS generator at 32kHz.
//
// We update two phase accumulators and use the upper 8 bits
// as the phase index to the sine table for each.  We then mix
// the two tones to generate DTMF audio and remap signal level.
// The pair of tones will be generated anytime timer 2 is enabled.
//
ISR(TIMER2_OVF_vect)
{
    volatile unsigned int phase_low, phase_high;
    int summed_phase;
    
    // Update phase accumulators and extract the sine table index from them
    //
    phase_accumulator_low  += rgtuning_word[tone_low];
    phase_accumulator_high += rgtuning_word[tone_high];

    // Use the upper 8 bits as index into sine table
    //
    sine_table_index = phase_accumulator_low  >> 24;
    phase_low = pgm_read_byte_near(sine256 + sine_table_index);
    
    sine_table_index = phase_accumulator_high >> 24;
    phase_high = pgm_read_byte_near(sine256 + sine_table_index);
    
    // Add the phase points for the high and low tone together and remap to 0-255
    // in order to mix the two tones.
    //
    summed_phase = (int) phase_low + (int) phase_high;
    OCR2A = (unsigned char) map(summed_phase, 0, 510, 0, 255);
}
