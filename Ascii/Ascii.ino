// Ascii.ino - A simple routine to print an ascii table to the serial port
//
// https://en.wikipedia.org/wiki/ASCII
// 
// http://www.asciitable.com/
// https://en.wikipedia.org/wiki/ASCII
//
// Copyright 2005 Jeff Whitlatch - ko7m

#define count(x) (sizeof(x) / sizeof(x[0]))

void setup()
{
	char *label[] = { ", dec: ", ", hex: ", ", oct: ", ", bin: " };
	int conv[] = { DEC, HEX, OCT, BIN };

	//Initialize serial and wait for port to open:
	Serial.begin(115200);

	Serial.println("ASCII Table ~ Character Map");
	for (int iChar = '!'; iChar <= '~'; iChar++)
	{
		Serial.write(iChar);
		for (int i = 0; i < count(conv); i++)
		{
			Serial.print(label[i]);
			Serial.print(iChar, conv[i]);
		}
		Serial.println();
	}
}

void loop()
{
	// Nothing to do in main loop, just hang the script
}
