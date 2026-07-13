void setup() {
#ifndef USE_4028
    Serial.begin(115200);               // Comment this out if GPIO pins are needed
    Serial.println(F("KO7M radio controller"));
    Serial.println();
#endif // USE_4028
}

void loop() {
    // put your main code here, to run repeatedly:

}
