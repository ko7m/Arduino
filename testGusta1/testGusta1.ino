#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>
#include <Wire.h>


char server[] = "www.google.com";
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 177);
IPAddress myDns(192, 168, 0, 1);
EthernetClient client;

unsigned long beginMicros, endMicros;
unsigned long byteCount = 0;
bool printWebData = false;
unsigned long pulse2;
char buf[20];
unsigned long pulse;
int hourValue;
int dayValue;
int ledState7;
const byte sdChip = A0;
const byte sensorPin = 2;
const byte sensorPin2 = 3;
const byte ledPin2 = 8;
const byte pin = 7;
volatile byte state = LOW;
unsigned long interval2 = 0;
unsigned long litrosHora;
unsigned long litrosServidor = 0;
unsigned long startTime;
boolean pinValue = false;
unsigned static long lastPulse = 0;
unsigned static long lastPulse2 = 0;
unsigned long interval = 0;
unsigned long startTimeOfOneDayWait;
unsigned long intervalOfOneDay = 30000;
unsigned long startTimeOfOneHourWait;
unsigned long intervalOfOneHour = 15000;

void setup() {
  Serial.begin(115200);
  pulse = 1;
  sprintf(buf, "%ld", pulse);
  Wire.begin(8);


  Serial.println(F("Initialize Ethernet with DHCP:"));
  if (Ethernet.begin(mac) == 0) {
    Serial.println(F("Failed to configure Ethernet using DHCP"));

    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println(F("Ethernet shield was not found"));
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println(F("Ethernet cable is not connected."));
    }

    Ethernet.begin(mac, ip, myDns);
  } else {
    Serial.print(F("  DHCP assigned IP "));
    Serial.println(Ethernet.localIP());
  }

  Serial.print(F("connecting to "));
  Serial.print(server);
  Serial.println(F("..."));

  if (client.connect(server, 80)) {
    Serial.print(F("connected to "));
    Serial.println(client.remoteIP());
  }

  beginMicros = micros();

  attachInterrupt(digitalPinToInterrupt(sensorPin), pulseCounter, FALLING);
  litrosServidor = random(10, 20);
  litrosHora = 8;
  startTimeOfOneDayWait = millis();
  startTimeOfOneHourWait = millis();
  startTime = millis();

  attachInterrupt(digitalPinToInterrupt(sensorPin2), pulseCounter2, FALLING);

  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(sensorPin, INPUT_PULLUP);
  randomSeed (analogRead (0));
  Serial.println(F("Sem pulsos"));

}



void loop() {
  limitadorPulsoHora();
  limitadorPulso();
  if (millis() - startTimeOfOneHourWait >= intervalOfOneHour) {

    hourValue = 1;
    Serial.println(F("one hour passed"));
    startTimeOfOneHourWait = millis();
  } else {
    hourValue = 0;

  }
  if (millis() - startTimeOfOneDayWait >= intervalOfOneDay) {
    dayValue = 1;
    Serial.println(F("one day passed"));
    startTimeOfOneDayWait = millis();
  } else {
    dayValue = 0;
  }


  if (pulse != lastPulse)
  {
    Serial.print(pulse, DEC);
    Serial.print(F(" pulse"));
    if (pulse > 1) {

      Serial.print(F("s"));

    }
    Serial.println(F(" detected."));
    lastPulse = pulse;
  }

  if (pulse2 != lastPulse2)
  {
    Serial.print(pulse2, DEC);
    Serial.print(F(" pulse2"));
    if (pulse2 > 1) {

      Serial.print(F("s"));

    }
    Serial.println(F(" detected."));
    lastPulse2 = pulse2;
  }
}



void pulseCounter()
{
  pulse++;

}
void pulseCounter2()
{
  pulse2++;
}



void limitadorPulsoHora() {
  if (pulse >= litrosHora) {

    Serial.println(F("one hour has passed"));
    digitalWrite(8, HIGH);
    pulseReset2();
    if (digitalRead(pin) == HIGH) {
      digitalWrite(7, LOW );
      Serial.println(F("relay loop terminated hourly"));

    }

    if (hourValue == 0) {
      funcaoRelay();
    }
  }
}

void limitadorPulso() {

  if (pulse >= litrosServidor) {
    digitalWrite(8, HIGH);
    Serial.println(F("daily ammount reached "));
    pulseReset();
    if (digitalRead(pin) == HIGH) {
      digitalWrite(7, LOW);
      Serial.println(F("relay loop terminated daily"));
    }

  } if (hourValue == 0) {
    funcaoRelay();
  }

}

void funcaoRelay() {

  if (digitalRead(ledPin2) == HIGH) {
    digitalWrite(8, LOW);
    Serial.println(F("limiter terminated relay loop starts"));
  }
  unsigned long elapsed;

  elapsed = millis() - startTime;
  if (elapsed > interval)
  {

    digitalWrite(pin, pinValue = !pinValue);
    interval = random(3000, 6000);
    startTime = millis();

  }


}



void pulseReset() {
  if (dayValue == 1) {
    pulse = 1;
    Serial.println(F("pulse reset after day value"));

  } else {

  }
}
void pulseReset2() {
  if (hourValue == 1) {
    pulse = 1;
    Serial.println(F("pulse reset after hour value"));
  }
}

void printEthernet() {

  if (!client.connected()) {
    endMicros = micros();
    Serial.println();
    Serial.println(F("disconnecting."));
    client.stop();
    Serial.print(F("Received "));
    Serial.print(byteCount);
    Serial.print(F(" bytes in "));
    float seconds = (float)(endMicros - beginMicros) / 1000000.0;
    Serial.print(seconds, 4);
    float rate = (float)byteCount / seconds / 1000.0;
    Serial.print(F(", rate = "));
    Serial.print(rate);
    Serial.print(F(" kbytes/second"));
    Serial.println();


  } else {
    if (client.connect(server, 80)) {
      Serial.print(F("connected to "));
      Serial.println(client.remoteIP());

      client.println(F("GET /search?q=arduino HTTP/1.1"));
      client.println(F("Host: www.google.com"));
      client.println(F("Connection: close"));
      client.println();
    } else {
      Serial.println(F("connection failed"));
    }
  }
}

void sendEvent() {


  Wire.write(buf);






}
