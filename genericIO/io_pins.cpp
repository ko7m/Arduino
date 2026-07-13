// io_pins.cpp - Middle layer driver for I/O pins.
//
// Abstracts away details like whether the pins is on a GPIO or I/O expander.
//

#include "io_pins.h"

// Define the I/O pins the app will use here
//
IO_PIN LED_PIN("LED", GPIO_PIN, 13, IOPIN_DIR_OUTPUT, IOPIN_ACTIVE_HI); 

// Convert the I/O pin state to pin value.  This allows for active high/low to
// be abstracted.
//
inline bool IO_PIN::state2pinval(bool state)
{
    return _logic == IOPIN_ACTIVE_HI ? state : !state;
}

// Convert the pin value to a logical state
//
inline bool IO_PIN::pinval2state(bool pinVal)
{
    return _logic == IOPIN_ACTIVE_HI ? pinVal : !pinVal;
}

// Constructor for raw GPIO pins
//
IO_PIN::IO_PIN(const char *name, io_pin_raw_t type, int pin, io_pin_dir_t dir, io_pin_logic_t logic)
{
    _state = false;
    _pin_function_name = name;
    _device_type = type;
    //_xpander_instance = 0;
    _pin_num.gpio_pin_num = pin;
    pinMode(pin, _direction = dir);
    _logic = logic;
}

// Fetch the pin name
//
const char * IO_PIN::getName(void)
{
    return _pin_function_name;
}

// Set the pin I/O direction
//
void IO_PIN::setDirection(io_pin_dir_t direction)
{
    switch (_device_type)
    {
        case GPIO_PIN:
            pinMode(_pin_num.gpio_pin_num, 
                    direction == IOPIN_DIR_OUTPUT ? OUTPUT : INPUT);
            break;

        case IO_EXPANDER:
            break;
            
        default:
            break;
    }
}

// Get the current I/O pin direction setting
//
io_pin_dir_t IO_PIN::getDirection(void)
{
    return _direction;
}

// Make sure that the state of the hardware pin matches the logical state of the driver
//
void IO_PIN::flushSettings(bool generateEvents)
{
    switch(_device_type)
    {
    case GPIO_PIN:
        break;
        
    case IO_EXPANDER:
        break;
    }
}

// The meaning of "state" here is true=asserted, false=not asserted
// This class abstracts active high and active low so the user
// only need think of that when instantiating the object.
//
void IO_PIN::setState(bool state)
{
    uint8_t pinval = this->state2pinval(state);

    switch (_device_type)
    {
    case GPIO_PIN:
        // use low level gpio driver
        //
        digitalWrite(_pin_num.gpio_pin_num, pinval);
        break;
    case IO_EXPANDER:
        break;
    default:
        break;
    }
    _state = state;
}

// Retrieve the current pin state
//
bool IO_PIN::getState(void)
{
    return _state;
}

// Toggle the current state of the pin
//
void IO_PIN::toggleState(void)
{
    setState(!_state);
}
