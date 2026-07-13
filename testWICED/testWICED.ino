void setup()
{
    pinMode(PA15, OUTPUT);
}

void loop()
{
    digitalWrite(PA15, 1);
    delay(500);
    digitalWrite(PA15, 0);
    delay(500);
}

