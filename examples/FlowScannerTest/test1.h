

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#include <avr/pgmspace.h>
#include "printf.h"
#else
#if defined(WIN32) || defined(UNIX)

#include <inttypes.h>
#include <iostream>

#define PSTR(x) x
#define pgm_read_byte(str) (*(str))
#define PGM_P const char*
#define PROGMEM

#define min(x,y) ((x) < (y)? (x) : (y))

#endif
#endif
void test1();