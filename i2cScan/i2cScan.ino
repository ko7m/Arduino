// i2c_scan - i2c scanner variant that expects a PCA9546 multiplexer on the bus
//
// This i2c scanner is a variant on my original scanner that expects to find a
// PCA9546 i2c multiplexer.  We first scan with channel 0 selected which means
// the PCA9546 does not pass traffic to any of the four multiplexed busses.
// The effect is the same as if the multiplexer was not in the circuit.  Then,
// each of the four multiplexer channels is selected and the scan is repeated.
// You will see all i2c device both on the input to the multiplexer (including
// the multiplexer itself) and all devices on the far side of the multiplexer
// channel selected.  Here is an example of the output where there is an LCD
// and the PCA9546 on the input, an Si570 on Channel 1 and a second Si570 on
// Channel 2.  Remember you will see for each channel not only the devices
// connected to that channel, but also the devices on the input to the mux.
//
// i2c Scanner
// Channel 0
// Scanning...
// i2c device found at address 0x20
// i2c device found at address 0x70
//
// Channel 1
// Scanning...
// i2c device found at address 0x20
// i2c device found at address 0x55
// i2c device found at address 0x70
//
// Channel 2
// Scanning...
// i2c device found at address 0x20
// i2c device found at address 0x55
// i2c device found at address 0x70
//
// Channel 3
// Scanning...
// i2c device found at address 0x20
// i2c device found at address 0x70
//
// Channel 4
// Scanning...
// i2c device found at address 0x20
// i2c device found at address 0x70
//
// Jeff Whitlatch - ko7m

#include <Wire.h>
#include "PCA9546.h"

PCA9546 *mux;		// Declare the multiplexer object

// Print the i2c device found as a two digit hex address
void printAddress(byte address)
{
	if (address<16) Serial.print("0");
	Serial.println(address, HEX);
}

// Set up the wire library and 
void setup()
{
	uint8_t nChannel;

	Wire.begin();

	Serial.begin(115200);
	Serial.println("i2c Scanner");
	mux = NULL;

	// Iterate through all multiplexer channels including the case
	// where no output channels are enabled.
	for (nChannel = 0; nChannel <= 4; nChannel++)
	{
		Serial.print("Channel ");
		Serial.println(nChannel, DEC);
		byte error, address;
		int nDevices;

		Serial.println("Scanning...");

		nDevices = 0;
		if (nChannel > 0)
		{
			if (mux == NULL)
			{
				mux = new PCA9546(0x70, 1 << (nChannel - 1));
			}
			else
			{
				bool fRc = mux->selectChannel(1 << (nChannel - 1));
			}
		}

		for(address = 1; address < 127; address++ ) 
		{
			Wire.beginTransmission(address);
			error = Wire.endTransmission();

			// Just for good measure, try again
			if (error != 0)
			{
				delay(10);
				Wire.beginTransmission(address);
				error = Wire.endTransmission();
			}

			if (error == 0)
			{
				Serial.print("i2c device found at address 0x");
				printAddress(address);  
				nDevices++;
			}
			else if (error == 4) 
			{
				Serial.print("Unknown error at address 0x");
				printAddress(address);
			}    
		}
		if (nDevices == 0)
			Serial.println("No i2c devices found\n");
		else
			Serial.println("");
	}
}

void loop()
{
	// Nothing to do in the main loop, just hang the device
}
