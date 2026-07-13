// testTypeId.cpp
//

#include <stdio.h>
#include <stdint.h>

void setup() {
  Serial.println("         char is: " + typeid(char).name()          );
  Serial.println("         int  is: " + typeid(int).name()           );
  Serial.println("     unsigned is: " + typeid(unsigned).name()      );
  Serial.println("         long is: " + typeid(long).name()          );
  Serial.println("    long long is: " + typeid(long long).name()     );
  Serial.println("unsigned long is: " + typeid(unsigned long).name() );
  Serial.println("        float is: " + typeid(float).name()         );
  Serial.println("       double is: " + typeid(double).name()        );
  Serial.println("      uint8_t is: " + typeid(uint8_t).name()       );
  Serial.println("     uint16_t is: " + typeid(uint16_t).name()      );
  Serial.println("     uint32_t is: " + typeid(uint32_t).name()      );
  Serial.println("     uint64_t is: " + typeid(uint64_t).name()      );
}

void loop()
{
    
}

