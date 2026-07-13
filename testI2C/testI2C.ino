#include <LoRaNode.h>
#include <config.h>

#include <SPI.h>
//#include <LoRa.h>
#include <Wire.h>

#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26
#define BAND 915E6

const byte buttonPin = 17;
unsigned long counter = 1;
unsigned long startTime;
unsigned long interval = 10000;
unsigned long interval2 = 20000;
int buttonState;
int binValue;
void setup() {
    Serial.begin(9600);
    SPI.begin(SCK, MISO, MOSI, SS);
    LoRa.setPins(SS, RST, DIO0);
    pinMode(17, INPUT);

    if (!LoRa.begin(BAND)) {
        Serial.println("Starting LoRa failed!");
        while (1);
    }

    Serial.println("LoRa Initializing OK!");
    startTime = millis();
    counter++;
}

void loop() {
    statusRelay = digitalRead(17);
    if( statusRelay == HIGH )
    {
        binValue = 1;
    }
    else{
        binValue = 0; 
    }
    sendPacket(statusRelay);
}

void sendPacket(int status)
{
    // envia um pacote
    LoRa.beginPacket(); 
    LoRa.print("="); //delimitador
    LoRa.print(status); //1(ativo) ou 0(inativo)
    LoRa.print("#"); //delimitador
    LoRa.endPacket();
}
