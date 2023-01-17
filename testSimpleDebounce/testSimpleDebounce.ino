
byte debouncePin(byte digitalPin)
{
    static byte firstSample;
    static byte secondSample;
    
    do
    {
        firstSample = digitalRead(digitalPin);
        delay(50);
        secondSample = digitalRead(digitalPin);
    } while (firstSample != secondSample);
    return secondSample;
}

void setup() 
{
    pinMode(2, INPUT_PULLUP);
    pinMode(13, OUTPUT);
}

void loop()
{
    digitalWrite(13, debouncePin(2));
}
