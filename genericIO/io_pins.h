// io_pins.h
//

#pragma once

#include "Arduino.h"

// Type definitions for raw GPIO pins vs. I/O expander
//
typedef enum
{
  GPIO_PIN,
  IO_EXPANDER
} io_pin_raw_t;

// Type definitions for I/O direction
//
typedef enum
{
  IOPIN_DIR_INPUT,
  IOPIN_DIR_OUTPUT
} io_pin_dir_t;

// Type definition for generic I/O pins that can be either on
// physical I/O pins or I/O expanders.
//
typedef union io_pin_u_
{
  uint8_t          gpio_pin_num;       // Hard I/O pins
  //io_port_pins_t  io_port_pin_num;    // I/O expander pins
} io_pinnum_ut;

// Logic active high/low definitions
//
typedef enum
{
  IOPIN_ACTIVE_HI,
  IOPIN_ACTIVE_LOW
} io_pin_logic_t;

// IO pin class definition
//
class IO_PIN
{
public:
    IO_PIN(const char *name, io_pin_raw_t type, int pin_num, io_pin_dir_t dir, io_pin_logic_t logic);  // Hardware GPIO
    //IO_PIN(const char *name, io_pin_raw_type, IO_EXPANDER_t  *xpander_instance, io_port_pins_t pin_num, io_pin_dir_t dir, io_pin_logic_t logic); // Expander GPIO

    const char *   getName(void);
    void           setDirection(io_pin_dir_t direction);
    io_pin_dir_t   getDirection(void);
    void           flushSettings(bool generateEvents);
    void           setState(bool state);
    bool           getState(void);
    void           toggleState(void);
    io_pin_logic_t getLogic(void);
    //void           emitEvent(void);

    // Implement IEventListener
    //
    //void           onEvent(event_t *);

    // Implement IIRQListener
    //
    //void           onIRQ(any_ptr);

private:
    bool _state = false;
    const char *      _pin_function_name = "NOT_NAMED";
    io_pin_raw_t      _device_type = GPIO_PIN;
    io_pin_dir_t      _direction = IOPIN_DIR_INPUT;
    //gpioPort_t      _base = nullptr;
    //IO_EXPANDER_t * _xpander_instance = nullptr;
    io_pinnum_ut      _pin_num;   // GPIO it is the digital pin number, I/O expander it is the pin within the expander
    io_pin_logic_t    _logic = IOPIN_ACTIVE_HI;
    inline bool       state2pinval(bool state);
    inline bool       pinval2state(bool pinVal);
};

// Forward declarations for defined pins
//
extern IO_PIN LED_PIN; 
