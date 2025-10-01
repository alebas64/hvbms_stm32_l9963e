#ifndef NTC_H
#define NTC_H
#include "stdint.h"
#include "math.h"

#define VCC        3.3
#define R_FIXED    10000.0
#define BETA       3950.0
#define T0         298.15  // 25°C in Kelvin
#define R0         10000.0
#define RESOLUTION ((2^16)-1)

int16_t ntc_get_centideg_from_raw(uint16_t);

#endif // NTC_H
