const byte sensorPin = 2;
const byte ledPin2 = 8;
const byte pin = 7;
volatile byte state = LOW;
long interval2 = 0; 
long pulse = 0;
long litrosServidor = 0;
unsigned long startTime;
long var = 0;
boolean pinValue = false;
int ledState = LOW;
static long lastPulse = 0;
long interval =0;
void pulseCounter();

void setup() {
  attachInterrupt(digitalPinToInterrupt(sensorPin), pulseCounter, FALLING);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(sensorPin, INPUT_PULLUP);

 

    randomSeed (analogRead (0));


litrosServidor = random(10,20);
startTime = millis();




  Serial.begin(115200);
  Serial.println(F("Sem pulsos"));


}


void loop(){
  limitadorPulso();

    if (pulse != lastPulse)
    {
        Serial.print(pulse, DEC);
        Serial.print(F(" pulse"));
        if (pulse > 1){
            Serial.print(F("s"));
            Serial.println(F(" detected."));
            lastPulse = pulse;
        }
    }
}




  void pulseCounter()
{
    pulse++;

}







 inline void limitadorPulso(){
 
 if(pulse >= litrosServidor){
  digitalWrite(8, HIGH);

  esperaDia();


}else{
funcaoRelay();
 

}

}

inline void funcaoRelay(){
 

    int elapsed;

    elapsed = millis() - startTime;
    if (elapsed > interval)
    {



        digitalWrite(pin, pinValue = !pinValue);
        interval = random(3000, 6000);
        Serial.println(interval, DEC);
    startTime = millis();

    }


}



inline void tempoReset(){
 


 



}

inline void esperaDia(){
  delay(10000);
   pulse = 0;

}
