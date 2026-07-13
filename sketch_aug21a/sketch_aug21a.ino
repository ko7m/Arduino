void setup() {
    Serial.begin(115200);

}

void loop() {
    static float test = 0.0;
    Serial.println(test);
    test += 0.1;

}
