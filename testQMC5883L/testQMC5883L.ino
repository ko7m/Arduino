// Jeffs_Mag_01.cpp

#include <Wire.h> //I2C Arduino Library

#define addr 0x0D //I2C Address for The HMC5883

void setup() {

  Serial.begin(115200);
  Serial.print("\r\n## Booting\r\n");
  
  Wire.begin();

  Wire.beginTransmission(addr); //start talking
  Wire.write(0x0B); // Tell the HMC5883 to Continuously Measure
  Wire.write(0x01); // Set the Register
  Wire.endTransmission();
  Wire.beginTransmission(addr); //start talking
  Wire.write(0x09); // Tell the HMC5883 to Continuously Measure
  Wire.write(0x1D); // Set the Register
  Wire.endTransmission();
}

void loop() {

  int16_t x, y, z; //triple axis data
  int16_t t; // Temperature
  uint8_t status = 0;

  do
  {
    Wire.beginTransmission(addr);
    Wire.write(0x06);
    Wire.endTransmission();
    Wire.requestFrom(addr, 1);
    status = Wire.read();
  
  } while (0 == (status & 1));  

  Wire.beginTransmission(addr);
  Wire.write(0x00); //start with register 0.
  Wire.endTransmission();

  //Read the data.. 2 bytes for each axis.. 6 total bytes
  Wire.requestFrom(addr, 9);
  if (9 <= Wire.available()) {
    x = Wire.read(); //LSB  x
    x |= Wire.read() << 8; //MSB  x
    
    y = Wire.read(); //LSB y
    y |= Wire.read() << 8; //MSB y
    
    z = Wire.read(); //LSB  z
    z |= Wire.read() << 8; //MSB z

    status = Wire.read();  
    
    t = Wire.read(); //LSB  t
    t |= Wire.read() << 8; //MSB t
    
  }


  // Show Values
  Serial.print("  X: "); Serial.print(x);
  Serial.print(", Y: "); Serial.print(y);
  Serial.print(", Z: "); Serial.print(z);

  { // Added by: ERB
    int a = atan2((double)y,(double)x) * (180 / 3.14159265);    // angle in degrees
    Serial.print(", A: "); Serial.print(a);
    //Serial.print(", Tc: "); Serial.println(t/100.0-605,1); // Temperature C
    //Serial.print(", Tc: "); Serial.println(-t+1); // Temperature C (is two's complement)
    //Serial.print(", Tc: "); Serial.println((~t+1)/100.0); // Temperature C
    //Serial.print(", Tc: "); Serial.println(t/100.0, DEC); // Temperature C
    Serial.print(", Temp: "); Serial.print(t/100.0+44, 2); Serial.println("C"); // Temperature C
  }

  delay(1000);
}

// End
