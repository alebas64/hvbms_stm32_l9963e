#include "can_utils.h"

pack_information_t *pack_information_pointer = NULL;
void can_routines_init(pack_information_t *ptr){
    pack_information_pointer = ptr;
}

void can_send_routine(){
    if(pack_information_pointer == NULL)
        return;
    CAN_TxHeaderTypeDef tx_header;
    uint8_t send_index = 0;
    uint8_t i = 0;
    uint8_t flag_holder = 0;
    uint8_t buffer[8];

    //*cell voltages
    tx_header.ExtId = 1;
    tx_header.IDE   = CAN_ID_EXT;
    tx_header.RTR   = CAN_RTR_DATA;
    tx_header.StdId = CAN_PACKET_BMS_V_CELL;
    tx_header.DLC   = 8;
    
    while(i < L9963Es_TOTAL_CELLS_N){
        send_index = 0;
        buffer[send_index++] = i;
        buffer[send_index++] = L9963Es_TOTAL_CELLS_N;

        if (i < L9963Es_TOTAL_CELLS_N) {
            libBuffer_append_uint16(buffer, pack_information_pointer->cell_voltages_mV[i], &send_index);
        }
        if (i < L9963Es_TOTAL_CELLS_N) {
            libBuffer_append_uint16(buffer, pack_information_pointer->cell_voltages_mV[i], &send_index);
        }
        if (i < L9963Es_TOTAL_CELLS_N) {
            libBuffer_append_uint16(buffer, pack_information_pointer->cell_voltages_mV[i], &send_index);
        }
        can_send_msg(buffer, &tx_header);
        HAL_Delay(2); //small delay for not saturating the CANBUS
    }
    
    //*cell temperatures
    tx_header.ExtId = 1;
    tx_header.IDE   = CAN_ID_EXT;
    tx_header.RTR   = CAN_RTR_DATA;
    tx_header.StdId = CAN_PACKET_BMS_TEMPS;
    tx_header.DLC   = 8;
    i=0;
    while(i < L9963Es_TOTAL_GPIOS_N){
        send_index = 0;
        buffer[send_index++] = i;
        buffer[send_index++] = L9963Es_TOTAL_GPIOS_N;

        if (i < L9963Es_TOTAL_GPIOS_N) {
            libBuffer_append_uint16(buffer, pack_information_pointer->cell_temperatures_centideg[i], &send_index);
        }
        if (i < L9963Es_TOTAL_GPIOS_N) {
            libBuffer_append_uint16(buffer, pack_information_pointer->cell_temperatures_centideg[i], &send_index);
        }
        if (i < L9963Es_TOTAL_GPIOS_N) {
            libBuffer_append_uint16(buffer, pack_information_pointer->cell_temperatures_centideg[i], &send_index);
        }
        can_send_msg(buffer, &tx_header);
        HAL_Delay(2); //small delay for not saturating the CANBUS
    }

    //*packvoltage
    tx_header.ExtId = 1;
    tx_header.IDE   = CAN_ID_EXT;
    tx_header.RTR   = CAN_RTR_DATA;
    tx_header.StdId = CAN_PACKET_BMS_V_TOT;
    tx_header.DLC   = 4;
    send_index=0;
    libBuffer_append_uint32(buffer, pack_information_pointer->pack_voltage_mV, &send_index);
    can_send_msg(buffer, &tx_header);

    //*pack current
    tx_header.ExtId = 1;
    tx_header.IDE   = CAN_ID_EXT;
    tx_header.RTR   = CAN_RTR_DATA;
    tx_header.StdId = CAN_PACKET_BMS_I;
    tx_header.DLC   = 4;
    send_index=0;
    libBuffer_append_uint32(buffer, pack_information_pointer->current_mA, &send_index);
    can_send_msg(buffer, &tx_header);

    //*CAN_PACKET_BMS_VOLTAGE_STAT
    tx_header.ExtId = 1;
    tx_header.IDE   = CAN_ID_EXT;
    tx_header.RTR   = CAN_RTR_DATA;
    tx_header.StdId = CAN_PACKET_BMS_VOLTAGE_STAT;
    tx_header.DLC   = 6;
    send_index=0;
    libBuffer_append_uint16(buffer, pack_information_pointer->cell_min_voltage_mV, &send_index);
    libBuffer_append_uint16(buffer, pack_information_pointer->cell_max_voltage_mV, &send_index);
    libBuffer_append_uint16(buffer, pack_information_pointer->cell_voltage_avg_mV, &send_index);
    libBuffer_append_uint8(buffer, 100, &send_index);
    can_send_msg(buffer, &tx_header);

    //*CAN_PACKET_BMS_TEMPERATURE_STAT
    tx_header.ExtId = 1;
    tx_header.IDE   = CAN_ID_EXT;
    tx_header.RTR   = CAN_RTR_DATA;
    tx_header.StdId = CAN_PACKET_BMS_TEMPERATURE_STAT;
    tx_header.DLC   = 6;
    send_index=0;
    libBuffer_append_uint16(buffer, pack_information_pointer->cell_min_temperature_centideg, &send_index);
    libBuffer_append_uint16(buffer, pack_information_pointer->cell_max_temperature_centideg, &send_index);
    libBuffer_append_uint16(buffer, pack_information_pointer->cell_temperature_avg_centideg, &send_index);
    can_send_msg(buffer, &tx_header);
    
    //*CAN_PACKET_BMS_VOLTAGE_STAT
    tx_header.ExtId = 1;
    tx_header.IDE   = CAN_ID_EXT;
    tx_header.RTR   = CAN_RTR_DATA;
    tx_header.StdId = CAN_PACKET_BMS_STAT;
    tx_header.DLC   = 3;
    send_index=0;
    libBuffer_append_uint8(buffer, 100, &send_index); //SoC
    libBuffer_append_uint8(buffer, 100, &send_index); //SoH
    flag_holder = 0;
    flag_holder |= (pack_information_pointer->battery_state.is_charging);
    flag_holder |= (pack_information_pointer->battery_state.is_discharging << 1);
    flag_holder |= (pack_information_pointer->battery_state.is_equalizing << 2);
    libBuffer_append_uint8(buffer, 0, &send_index); //flags
    can_send_msg(buffer, &tx_header);

    //*CAN_PACKET_BMS_W
    tx_header.ExtId = 1;
    tx_header.IDE   = CAN_ID_EXT;
    tx_header.RTR   = CAN_RTR_DATA;
    tx_header.StdId = CAN_PACKET_BMS_W;
    tx_header.DLC   = 8;
    send_index=0;
    libBuffer_append_uint64(buffer, pack_information_pointer->power_W, &send_index);
    can_send_msg(buffer, &tx_header);


    //*CAN_PACKET_BMS_BAL
    tx_header.ExtId = 1;
    tx_header.IDE   = CAN_ID_EXT;
    tx_header.RTR   = CAN_RTR_DATA;
    tx_header.StdId = CAN_PACKET_BMS_BAL;
    tx_header.DLC   = 8;
    send_index = 0;
    buffer[send_index++] = L9963Es_TOTAL_CELLS_N;
    uint64_t bal_state = 0;
    for (int i = 0; i < L9963Es_TOTAL_CELLS_N; i++) {
        bal_state |= (uint64_t)(pack_information_pointer->cell_balancing[i] == 0 ? 0 : 1) << i;
    }
    buffer[send_index++] = (bal_state >> 48) & 0xFF;
    buffer[send_index++] = (bal_state >> 40) & 0xFF;
    buffer[send_index++] = (bal_state >> 32) & 0xFF;
    buffer[send_index++] = (bal_state >> 24) & 0xFF;
    buffer[send_index++] = (bal_state >> 16) & 0xFF;
    buffer[send_index++] = (bal_state >> 8) & 0xFF;
    buffer[send_index++] = (bal_state >> 0) & 0xFF;
    can_send_msg(buffer, &tx_header);
}

void can_receive_routine(){
    //!REDACTED
}

void can_send_msg(uint8_t* buffer, CAN_TxHeaderTypeDef *tx_header){
    CAN_send(&hcan1, buffer, &tx_header);
}

void can_receive_msg(){
    //!REDACTED
}

void libBuffer_append_uint64(uint8_t* buffer,uint64_t data,uint8_t* index){
    buffer[(*index)++] = data >> 56;
	buffer[(*index)++] = data >> 48;
    buffer[(*index)++] = data >> 40;
	buffer[(*index)++] = data >> 32;
    buffer[(*index)++] = data >> 24;
	buffer[(*index)++] = data >> 16;
	buffer[(*index)++] = data >> 8;
	buffer[(*index)++] = data;
}

void libBuffer_append_uint32(uint8_t* buffer,uint32_t data,uint8_t* index){
    buffer[(*index)++] = data >> 24;
	buffer[(*index)++] = data >> 16;
	buffer[(*index)++] = data >> 8;
	buffer[(*index)++] = data;
}

void libBuffer_append_uint16(uint8_t* buffer,uint16_t data,uint8_t* index){
    buffer[(*index)++] = data >> 8;
	buffer[(*index)++] = data;
}

void libBuffer_append_uint8(uint8_t* buffer,uint8_t data,uint8_t* index){
    buffer[(*index)++] = data;
}