// app.cpp - Generic application class
//

#include "app.h"

// Return a singleton instance of the application class
//
APP *APP::getInstance()
{
    static APP _appInstance;
    return &_appInstance;
}

// Perform any application specific initialization
//
void APP::init()
{
    led = &LED_PIN;
}

// This is the main loop of the application.  
//
void APP::run()
{
    led->toggleState();
    delay(500);
}
