#ifndef INC_UTILS_H_
#define INC_UTILS_H_

#include "stdint.h"
#include "string.h"
#include <stdio.h>
#include "stm32f4xx_hal.h"

uint16_t max_search(uint16_t*, uint8_t);
uint16_t min_search(uint16_t*, uint8_t);
uint16_t get_avg(uint16_t*, uint8_t);
int16_t max_search_signed(int16_t*, uint8_t);
int16_t min_search_signed(int16_t*, uint8_t);
int16_t get_avg_signed(int16_t*, uint8_t);
uint8_t timer_check(uint32_t, uint32_t);

#endif /* INC_UTILS_H_ */