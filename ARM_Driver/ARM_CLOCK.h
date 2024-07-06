#ifndef __CLOCK__
#define __CLOCK__

#include <stdint.h>
#include <stdio.h>
#include "device_registers.h"

#define FAST_IRC			48000000

void Source_Clock(uint32_t source);

#endif
