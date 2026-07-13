// genericIO.ino - Test driver for generic IO class
//

#include "app.h"

void setup() 
{
    APP::getInstance()->init();
}

void loop() 
{
    APP::getInstance()->run();
}
