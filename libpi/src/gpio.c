/*
 * Implement the following routines to set GPIO pins to input or output,
 * and to read (input) and write (output) them.
 *
 * DO NOT USE loads and stores directly: only use GET32 and PUT32
 * to read and write memory.  Use the minimal number of such calls.
 *
 * See rpi.h in this directory for the definitions.
 */
#include "rpi.h"

// see broadcomm documents for magic addresses.
enum {
    GPIO_BASE = 0x20200000,
    gpio_set0  = (GPIO_BASE + 0x1C),
    gpio_clr0  = (GPIO_BASE + 0x28),
    gpio_lev0  = (GPIO_BASE + 0x34)
};

//
// Part 1 implement gpio_set_on, gpio_set_off, gpio_set_output
//

// set <pin> to be an output pin.
//
// note: fsel0, fsel1, fsel2 are contiguous in memory, so you
// can (and should) use array calculations!
void gpio_set_output(unsigned pin) {
  gpio_set_function(pin, GPIO_FUNC_OUTPUT);
  /*
    if(pin >= 32)
        return;
    unsigned offset = (pin / 10) * 4;
    unsigned value = GET32(GPIO_BASE + offset);
    unsigned mask = 0b111 << ((pin % 10) * 3);
    value &= ~mask;
    value |= 0x00000001 << ((pin % 10) * 3);
    PUT32(GPIO_BASE + offset, value);
    */

  // implement this
  // use <gpio_fsel0>
}

// set GPIO <pin> on.
void gpio_set_on(unsigned pin) {
    if(pin >= 32)
        return;
    PUT32(gpio_set0, 0x1 << pin);

  // implement this
  // use <gpio_set0>
}

// set GPIO <pin> off
void gpio_set_off(unsigned pin) {
    if(pin >= 32)
        return;
    PUT32(gpio_clr0, 0x1 << pin);
  // implement this
  // use <gpio_clr0>
}

// set <pin> to <v> (v \in {0,1})
void gpio_write(unsigned pin, unsigned v) {
    if(v)
        gpio_set_on(pin);
    else
        gpio_set_off(pin);
}

void gpio_set_function(unsigned pin, gpio_func_t function) {
    if(pin >= 32) {
          return;
    }
    if (function > 7) {
      return;
    }
      unsigned offset = (pin / 10) * 4;
      unsigned value = GET32(GPIO_BASE + offset);
      unsigned mask =  0b111 << ((pin % 10) * 3);
      value &= ~mask;
      value |= function << ((pin % 10) * 3);
      PUT32(GPIO_BASE + offset, value);
}

//
// Part 2: implement gpio_set_input and gpio_read
//

// set <pin> to input.
void gpio_set_input(unsigned pin) {
  // implement.
  gpio_set_function(pin, GPIO_FUNC_INPUT);
  /*
  if(pin >= 32)
        return;
        
    unsigned offset = (pin / 10) * 4;
    unsigned value = GET32(GPIO_BASE + offset);
    unsigned mask = 0b111 << ((pin % 10) * 3);
    value &= ~mask;
    value |= 0x000 << ((pin % 10) * 3);
    PUT32(GPIO_BASE + offset, value);
    */
}

// return the value of <pin>
int gpio_read(unsigned pin) {
  unsigned v = 0;
  v = GET32(gpio_lev0);
  v = v >> pin;
  v &= 0x1; 
  return DEV_VAL32(v);
}
