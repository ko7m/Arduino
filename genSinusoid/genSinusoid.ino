// Generate sinusoidal table data
//
// Jeff Whitlatch - ko7m
//

// Map from one range to another
//
static double map(double x, double in_min,
                  double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// One time processing goes here
//
void setup()
{
    Serial.begin(115200);
    
    Serial.println("PROGMEM prog_uchar sine256[] = ");
    Serial.println("{");
    Serial.print("    ");
    
    for (int i = 0; i < 256; i++)
    {
        double angle = 360.0 / 256.0 * (double) i;
        double radians = angle * PI / 180.0;
        double sine = sin(radians);
    
        // Map range from -1 to +1 to 0 to 255
        //
        int amplitude = (int) round(map(sine, -1.0, 1.0, 0, 255));
        Serial.print(amplitude, DEC);
        Serial.print(i != 255 ? "," : "");
        
        if (i > 0 && (i+1) % 16 == 0)
        {
          Serial.println();
          Serial.print("    ");
        }
    }
    Serial.println("};");
}

void loop()
{
    
}


