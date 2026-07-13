// testEEPROM
//

#include <EEPROM.h>

void setup()
{
    Serial.begin(115200);
    Serial.print("EEProm size is ");
    Serial.println(EEPROM.length());
}

void loop() 
{

}
