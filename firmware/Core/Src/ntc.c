#include "ntc.h"

int16_t ntc_get_centideg_from_raw(uint16_t raw){
    float Vadc = (float)raw * (VCC / RESOLUTION);
    float Rntc = R_FIXED * (Vadc / (VCC - Vadc));
    float tempK = 1.0 / (1.0 / T0 + (1.0 / BETA) * log(Rntc / R0));
    return (int16_t)((tempK * 100) - 27315);
}