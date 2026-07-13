// test.h
//

#pragma once

#include "Arduino.h"

class myClass : public Print
{
public:
    virtual size_t write(uint8_t);
    virtual size_t write(const char *pstr);
    virtual size_t write(const uint8_t *pch, size_t cch);
    
private:
};

