//#include "test.h"

void setup() {
    Serial.begin(115200);
    
    Serial.print('a');
    Serial.print("Hello world!");
    Serial.print('a', HEX);
    Serial.println();
    Serial.printf("Hello %s\n", "world");

}

void loop() {


}
