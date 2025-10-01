#ifndef ERRORS_H
#define ERRORS_H

#include "stdint.h"

void error_set_overvoltage(uint8_t);
void error_reset_overvaltage(uint8_t);

void error_set_undervoltage(uint8_t);
void error_reset_undervoltage(uint8_t);

void error_set_undertemp(uint8_t);
void error_reset_undertemp(uint8_t);

void error_set_overtemp(uint8_t);
void error_reset_overtemp(uint8_t);


#endif //ERRORS_H