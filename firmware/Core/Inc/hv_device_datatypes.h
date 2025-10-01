#ifndef HV_DEVICE_DATATYPES_H
#define HV_DEVICE_DATATYPES_H

#include "stdint.h"

typedef struct{
    uint8_t charging_avaiable_states; //obc tell what state is avaiable for charging
    uint8_t charging_current_state;   //bms tell what state wants the obc to be in
    uint32_t charging_voltage;
    uint32_t charging_current;
}charging_obc_t;

typedef struct{
    uint8_t charging_avaiable; //mppt tell if they can output power or not
}charging_mppt_t;

typedef struct{
    uint8_t powertrain_requested_state; //request state by powertrain (by powertrain)
    uint8_t powertrain_avaiable_state;  //what state the powertrain can use (by BMS) 
    uint32_t maximum_power_avaiable_W;  //maximum power that the powertrain can use (by BMS)
}discharging_powertrain_t;

typedef struct{
    charging_obc_t obc;
    charging_mppt_t mppt;
    discharging_powertrain_t powertrain;
}hv_devices_states_requests_t;

//possibili stati della colonnina di ricarica
#define CHARGING_STATE_A 0x1
#define CHARGING_STATE_B 0x2
#define CHARGING_STATE_C 0x3
#define CHARGING_STATE_D 0x4
#define CHARGING_STATE_E 0x5
#define CHARGING_STATE_F 0x6

#endif