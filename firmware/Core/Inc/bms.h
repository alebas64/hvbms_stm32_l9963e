#ifndef BMS_H
#define BMS_H
#include "stdint.h"
#include "l9963e_utils.h"
#include "cell_specs.h"
#include "tim.h"
#include "usart.h"
#include "string.h"
#include <stdio.h>
#include "utils.h"
#include "bms_datatypes.h"
#include "hv_device_datatypes.h"

#define PACK_CHARGE_CAPACITY_mAh (CHARGE_CAPACITY_mAh * 28)
#define PACK_MAX_DISCHARGE_CURRENT (MAX_DISCHARGE_CURRENT * 28 * 0.8)
#define PACK_MAX_CHARGE_CURRENT (MAX_CHARGE_CURRENT * 28 * 0.8)
#define PACK_OVERVOLTAGE (117600)
#define PACK_UNDERVOLTAGE (78500)
#define MIN_CURRENT_FOR_CHARGING_STATE_mA (1000)
#define MIN_CURRENT_FOR_DISCHARGING_STATE_mA (1000)

#define PACK_CHARGING_VOLTAGE_mV (117600)
#define PACK_CHARGING_CURRENT_mA (60000)
#define PACK_CHARGING_TIME_RISE_WAIT_ms (200)
#define PACK_CHARGING_CURRENT_RISE_STEP_mA (200)
#define PACK_CHARGING_CURRENT_START_mA (1000)
#define PACK_CHARGING_CURRENT_END_mA (CHARGING_CURRENT_END_mA * 28)

//*used for error management
#define ERROR_PRESENT      (1)
#define ERROR_NOT_PRESENT  (0)

//*fan control parameters
//se viene richiesto un valore inferiore a FAN_MIN_DUTY, il duty verrà impostato a FAN_MIN_DUTY
#define FAN_MIN_DUTY 20
//se viene richiesto un valore duty compreso tra FAN_MAX_DUTY e 100, il duty verrà impostato a FAN_MAX_DUTY
#define FAN_MAX_DUTY 100
// if the delta betwen max and min is above this threshold, use the max temperature for fan speed control
#define IGNORE_AVG_TEMPERATURE_DELTA_THRESHOLD 500 
typedef enum{
    FAN_OFF =    0,
    FAN_LOW =    30,
    FAN_MEDIUM = 60,
    FAN_HIGH =   80,
    FAN_MAX =    100
}FAN_DUTY_CYCLE;

typedef enum{
    T_OFF =    (0*100),
    T_LOW =    (30*100),
    T_MEDIUM = (45*100),
    T_HIGH =   (50*100),
    T_MAX =    (55*100)
}TEMPERATURE_FAN_DUTY;

//*balancing define parameters
#define DEF_HIGH_V_START_mV     (4100)   //minimum voltage for starting balancing procedure
#define DEF_DELTA_START_mV      (150)    //minimum delta between max and min for starting balancing procedure
#define DEF_BALANCE_BAND_mV     (10)     //minimum difference between cell voltage and min cell voltage for starting the balancing of that cell
#define CELL_BALANCING_ON       (0b10)
#define CELL_BALANCING_OFF      (0)


//*definizione stati possibili batteria
typedef enum BATTERY_SYSTEM_STATES{
    HV_ON,
    HV_OFF,
    HV_PRECHARGE,
    HV_DISCHARGE,
    HV_BRUTAL_OFF,
    VAC_CHARGING
}BATTERY_SYSTEM_STATES_Type;

void bms_routines_init(pack_information_t *, battery_errors_t *, hv_devices_states_requests_t *);
void bms_routine();
void battery_fan_duty(uint8_t);
void gpiowrite(uint8_t,uint8_t);
uint8_t voltage_check();
uint8_t temperature_check();
uint8_t current_check();
uint8_t battery_operability();
void compute_max_deltas();
uint8_t check_hv_devices_requests(uint8_t);
void hv_output_change_state(uint8_t);
void temperature_control();
void balancing_procedure();
pack_information_t* get_pack_information();

//* ------------ communication with other HV devices

typedef enum{
    VAC_CHARGING_START,
    VAC_CHARGING_STOP,
    SOLAR_CHARGING_START,
    SOLAR_CHARGING_STOP,
    POWERTRAIN_START,
    POWERTRAIN_STOP
}HV_DEVICES_COMMAND_REQUESTS;

void vac_charging_procedure(uint8_t);
void solar_charging_procedure(uint8_t);
void powertrain_control(uint8_t);

#endif

/*
            prrrrrrrr

   |\__/,|   (`\
   |_ _  |.--.) )
   ( T   )     /
  (((^_(((/(((_/

  */