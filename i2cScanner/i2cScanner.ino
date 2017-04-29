// i2c_scanner - Scans i2c bus looking for devices and displays addresses found
//
// This simple i2c bus scanner will look for any connected and enabled i2c
// devices on the i2c bus.  Each device address found will be printed in hex.
//
// Jeff Whitlatch - ko7m

#include <Wire.h>

// Print out an i2c address as two hex digits
void printAddress(byte address)
{
    if (address<16) Serial.print("0");
    Serial.println(address, HEX);
}

// Initialize the wire library and serial port.  Scan the entire i2c address space
void setup()
{
    Wire.begin();

    Serial.begin(115200);
    Serial.println("i2c Scanner");

    // Scan the entire i2c address space looking for devices
    Serial.println("Scanning...");

    int nDevices = 0;

    for(int address = 1; address < 127; address++ ) 
    {
        Wire.beginTransmission(address);
        int error = Wire.endTransmission();

        // Just for good measure, try again on error
        if (error != 0)
        {
//          Serial.print("Error ");
//          Serial.println(error, DEC);
//          Serial.println("Retrying...");
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
        Serial.println("No i2c devices found");
    else
        Serial.println("");
}

void loop()
{
    // Nothing to do in the main loop, just hang the script
}
