#ifndef CAN_UTILS_H
#define CAN_UTILS_H

#include "can.h"
#include "bms_architecture_parameters.h"
#include "bms_datatypes.h"

#define CAN_PACKET_BMS_V_TOT            0x260A
#define CAN_PACKET_BMS_I                0x270A
#define CAN_PACKET_BMS_W                0x280A
#define CAN_PACKET_BMS_V_CELL           0x290A
#define CAN_PACKET_BMS_BAL              0x2A0A
#define CAN_PACKET_BMS_TEMPS            0x2B0A
#define CAN_PACKET_BMS_STAT             0x2C0A
#define CAN_PACKET_BMS_VOLTAGE_STAT     0x2D0A
#define CAN_PACKET_BMS_TEMPERATURE_STAT 0x2E0A


void can_routines_init(pack_information_t*);
void can_send_routine();
void can_receive_routine();
void can_send_msg(uint8_t* buffer, CAN_TxHeaderTypeDef *tx_header);
void can_receive_msg();

void libBuffer_append_uint64(uint8_t*,uint64_t,uint8_t*);
void libBuffer_append_uint32(uint8_t*,uint32_t,uint8_t*);
void libBuffer_append_uint16(uint8_t*,uint16_t,uint8_t*);
void libBuffer_append_uint8(uint8_t*,uint8_t,uint8_t*);

#endif // CAN_UTILS_H
