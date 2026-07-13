// app.h - Generic application infrastructure
//

#pragma once

#include "io_pins.h"

class APP
{
public:
    void init();
    void run();
    static APP *getInstance();
        
private:
    IO_PIN *led;
};
