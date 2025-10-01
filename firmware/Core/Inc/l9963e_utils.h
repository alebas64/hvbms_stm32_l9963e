#ifndef L9963E_UTILS_H
#define L9963E_UTILS_H

#include <inttypes.h>
#include "L9963E.h"
#include "stm32_if.h"
#include "ntc.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>
#include "bms_architecture_parameters.h"

#define L9963_VRES 0.000089f

//valori corrispondenti a errori di lettura
#define MAX_READING_RETRY_L9963E_GPIO   (5)
#define MAX_READING_RETRY_L9963E_CELLS  (10)
#define CELL_VOLTAGE_READ_ERROR         (7000)
#define CELL_TEMPERATURE_READ_ERROR     (-160)

#define ENABLED_CELLS ( L9963E_CELL1 |L9963E_CELL2 | L9963E_CELL3 | L9963E_CELL4 | L9963E_CELL5 | L9963E_CELL6 | L9963E_CELL7 | L9963E_CELL8 | L9963E_CELL9 | L9963E_CELL10 | L9963E_CELL11 | L9963E_CELL12 | L9963E_CELL13 | L9963E_CELL14)

void L9963E_utils_init(void);
void L9963E_utils_read_cells(uint8_t, uint8_t);
void L9963E_utils_get_cells(uint16_t*);
void L9963E_utils_get_gpios(int16_t*);
uint16_t L9963E_utils_get_cell_mv(uint16_t);
uint32_t L9963E_utils_get_batt_mv();
int32_t L9963E_utils_get_batt_mA();
void balancing_set(uint8_t*,uint8_t); 

#endif // L9963E_UTILS_H
