unsigned long currentTimeStamp = (unsigned long) 4294967295 - (unsigned long) 12;       // Our replacement for the millis counter for this example
unsigned long startTime        = 0;

// Replacement for millis function in Arduino
//
unsigned long getTimeStamp()
{
    return currentTimeStamp;
}

// Arduino setup function
//
void setup()
{
    Serial.begin(115200);
    Serial.println("Starting...");
    startTime = getTimeStamp();
}

// Arduino loop function
//
void loop()
{
    unsigned long interval = 7;

    Serial.print(getTimeStamp(), DEC);
    Serial.print(" ");
    if (getTimeStamp() - startTime >= interval)
    {
        Serial.println();
        Serial.println("Interval has passed");
        startTime = getTimeStamp();
    }
    delay(250);
    currentTimeStamp++;
}
