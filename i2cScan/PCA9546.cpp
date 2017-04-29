// PCA9546.cpp - I2C Multiplexer Library for Arduino
//
// Arduino library for PCA9546 i2c multiplexer.  The PCA9546 is a 4 channel i2c
// multiplexer.  It provides for several issues when connecting i2c devices to
// Arduino.  When connecting multiple devices with the same i2c address, put them
// on different PCA9546 channels.  Another issue addressed by this device is when
// different voltage devices need be attached.  Arduino Uno for example is a 5V
// device, but devices like the Si570 are 3V3 devices.  The PCA9546 inputs and
// outputs are 5v tolerant, so my solution to this is to run the input to the
// PCA9536 at 5V and place all 5V devices on the input with the MUX as long as
// all i2c addresses are unique.  Put 3V3 or 1V8 devices on separate outputs
// and use pull-ups to the appropriate voltage rail for the lower voltage devices.
//
// Copyright 2014 Jeff Whitlatch - ko7m

#include <Arduino.h>
#include <Wire.h>
#include "PCA9546.h"

// Initialize the PCA9546 and enable the channel(s) indicated
PCA9546::PCA9546(uint8_t PCA9546_address, uint8_t channel)
{ 
	i2c_address = PCA9546_address;

	Wire.begin();
	selectChannel(channel);
}

// Send a channel selection word to the PCA9546
bool PCA9546::selectChannel(uint8_t channel)
{
	// Sanity check value passed.  Only least significant 4 bits vali
	if (channel <= 0xf)
	{
		i2c_write(channel);
		status = PCA9546_SUCCESS;
	}
	else
	{
		status = PCA9546_ERROR;
	}
	return (PCA9546_SUCCESS == status);
}

// Write a byte to I2C device.  There is only a single register.  If multiple bytes written, last one wins.
void PCA9546::i2c_write(uint8_t data)
{
	Wire.beginTransmission(i2c_address);
	Wire.write(data);
	Wire.endTransmission();
}

// Read the one byte register from the I2C device
uint8_t PCA9546::i2c_read()
{
	uint8_t rdata = 0xFF;
	Wire.beginTransmission(i2c_address);
	Wire.requestFrom(i2c_address, (uint8_t)1);
	if (Wire.available()) rdata = Wire.read();
	return rdata;
}
