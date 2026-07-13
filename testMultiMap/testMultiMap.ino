
// Remap values according to a list of valid output values
// _in should contain ordered values.  If there are less than two
// entries, juse return the first value.  Likewise, if the value
// is greater than the last value, return the last value.

<template typename T>
T multiMap(int val, int* _in, T* _out, int size)
{
    int pos = 1;

    // Range check things
    if (val <= _in[0] || size < 2) return _out[0];
    if (val >= _in[size-1]) return _out[size-1];

    // Search the right interval.  For now this is a linear search
    // The first element has already been tested
    while(val > _in[pos]) pos++;

    // If the table entry is an exact match, we are done
    if (val == _in[pos]) return _out[pos];

    // Otherwise we interpolate the result from the nearest two points.
    return (val - _in[pos-1]) * (_out[pos] - _out[pos-1]) / (_in[pos] - _in[pos-1]) + _out[pos-1];
}

    
void setup()
{
int inData[11] = { 0, 10, 20, 30, 35, 36, 37, 38, 39, 40, 50 };
long outData[11] = {0, 100000, 200000, 300000, 350000, 360000, 370000, 380000, 390000, 400000, 500000 };
int result = -1;

    for (int i = 0; i < 11; i++)
    {
        result = multiMap(inData[i], &inData[0], &outData[0], sizeof(inData));
        Serial.print(i);
        Serial.print(": ");
        Serial.print(inData[i]);
        Serial.print(" = ");
        Serial.println(result);
    }
}

void loop()
{

}

