//*structs for monitoring battery state
#ifndef BMS_DATATYPES_H
#define BMS_DATATYPES_H

#include "stdint.h"
#include "bms_architecture_parameters.h"

typedef struct{
    uint8_t state_machine_state;
    uint8_t is_charging; //* 0 not charging, 1 charging
    uint8_t is_discharging; //* 0 not discharging, 1 discharging
    uint8_t is_balancing; //* 0 not equalizing, 1 equalizing
    uint8_t is_charging_allowed; //* 0 not allowed, 1 allowed
}battery_state_t;

typedef struct{
    uint32_t pack_voltage_mV;
    uint32_t pack_voltage_precharge_mV;
    uint32_t pack_voltage_postfuse_mV;
    uint16_t cell_voltages_mV[L9963Es_TOTAL_CELLS_N];
    uint16_t cell_min_voltage_mV;
    uint16_t cell_max_voltage_mV;
    int16_t cell_voltage_avg_mV;
    uint16_t pack_cell_delta_mV; // difference between max and min cell voltage in mV
    uint8_t cell_balancing[L9963Es_TOTAL_CELLS_N];
    int16_t cell_temperatures_centideg[L9963Es_TOTAL_GPIOS_N];
    int16_t cell_min_temperature_centideg;
    int16_t cell_max_temperature_centideg;
    int16_t cell_delta_temperature_centideg;
    int16_t cell_temperature_avg_centideg;
    int64_t current_mA;
    int64_t power_W;
    uint8_t cell_balancing_number;
    uint32_t totalcurrentdischarged_Ah;
    uint32_t totalcurrentcharged_Ah;
    battery_state_t battery_state;
}pack_information_t;

typedef struct{
    uint8_t cell_soft_overvoltage[L9963Es_TOTAL_CELLS_N];
    uint8_t cell_hard_overvoltage[L9963Es_TOTAL_CELLS_N];
    uint8_t cell_soft_undervoltage[L9963Es_TOTAL_CELLS_N];
    uint8_t cell_hard_undervoltage[L9963Es_TOTAL_CELLS_N];
    uint8_t pack_overvoltage;
    uint8_t pack_undervoltage;
    uint8_t battery_overcurrent_charge;
    uint8_t battery_overcurrent_discharge;
    uint8_t cell_soft_overtemp[L9963Es_TOTAL_GPIOS_N];
    uint8_t cell_hard_overtemp[L9963Es_TOTAL_GPIOS_N];
    uint8_t cell_soft_undertemp[L9963Es_TOTAL_GPIOS_N];
    uint8_t cell_hard_undertemp[L9963Es_TOTAL_GPIOS_N];
}battery_errors_t;

#endif