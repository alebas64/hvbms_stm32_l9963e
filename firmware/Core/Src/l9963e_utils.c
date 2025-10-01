#include "L9963E_utils.h"

L9963E_HandleTypeDef h9l;
uint16_t vcellsm[L9963E_SLAVES_N][L9963E_CELLS_N];
uint16_t vcells[L9963E_CELLS_N];
uint16_t vgpio[L9963E_SLAVES_N][L9963E_GPIOS_N];
uint16_t vtot[L9963E_SLAVES_N];
uint32_t vsumbatt[L9963E_SLAVES_N];
uint16_t slaves_cells[L9963Es_TOTAL_CELLS_N];
uint16_t cells_addresses[] = {L9963E_CELL1, L9963E_CELL2, L9963E_CELL3, L9963E_CELL4, L9963E_CELL5, L9963E_CELL6, L9963E_CELL7, L9963E_CELL8, L9963E_CELL9, L9963E_CELL10, L9963E_CELL11, L9963E_CELL12, L9963E_CELL13, L9963E_CELL14};
uint16_t gpio_addresses[] = {L9963E_GPIO3, L9963E_GPIO4, L9963E_GPIO5, L9963E_GPIO6, L9963E_GPIO9};
int16_t  vbattery_current;
 
const L9963E_IfTypeDef interfaceH = {
    .L9963E_IF_DelayMs = DelayMs,
    .L9963E_IF_GetTickMs = GetTickMs,
    .L9963E_IF_GPIO_ReadPin = GPIO_ReadPinH,
    .L9963E_IF_GPIO_WritePin = GPIO_WritePinH,
    .L9963E_IF_SPI_Receive = SPI_ReceiveH,
    .L9963E_IF_SPI_Transmit = SPI_TransmitH
};

const L9963E_IfTypeDef interfaceL = {
    .L9963E_IF_DelayMs = DelayMs,
    .L9963E_IF_GetTickMs = GetTickMs,
    .L9963E_IF_GPIO_ReadPin = GPIO_ReadPinL,
    .L9963E_IF_GPIO_WritePin = GPIO_WritePinL,
    .L9963E_IF_SPI_Receive = SPI_ReceiveL,
    .L9963E_IF_SPI_Transmit = SPI_TransmitL
};

void L9963E_utils_init(void) {
  L9963E_init(&h9l, interfaceL, L9963E_SLAVES_N);
  //L9963E_init(&h9l, interfaceH, L9963E_SLAVES_N);
  //L9963E_addressing_procedure(&h9l, 0b11, 0, 0, 1);
  L9963E_addressing_procedure(&h9l, 0b00, 1, 0, 1);

    //configurazione dei gpio sulle board slaves
    L9963E_RegisterUnionTypeDef gpio9_3_conf_reg = {.generic = L9963E_GPIO9_3_CONF_DEFAULT};
    gpio9_3_conf_reg.GPIO9_3_CONF.GPIO3_CONFIG = 0b00;
    gpio9_3_conf_reg.GPIO9_3_CONF.GPIO4_CONFIG = 00;
    gpio9_3_conf_reg.GPIO9_3_CONF.GPIO5_CONFIG = 00;
    gpio9_3_conf_reg.GPIO9_3_CONF.GPIO6_CONFIG = 00;
    gpio9_3_conf_reg.GPIO9_3_CONF.GPIO7_CONFIG = 01;
    gpio9_3_conf_reg.GPIO9_3_CONF.GPIO8_CONFIG = 00;
    gpio9_3_conf_reg.GPIO9_3_CONF.GPIO9_CONFIG = 00;
    gpio9_3_conf_reg.GPIO9_3_CONF.GPIO7_WUP_EN = 00;
    L9963E_DRV_reg_write(&(h9l.drv_handle), L9963E_DEVICE_BROADCAST, L9963E_GPIO9_3_CONF_ADDR, &gpio9_3_conf_reg, 10);

    // Configuring cells overvoltage/undervoltage thresholds
    L9963E_RegisterUnionTypeDef vcell_thresh_uv_ov_reg      = {.generic = L9963E_VCELL_THRESH_UV_OV_DEFAULT};
    vcell_thresh_uv_ov_reg.VCELL_THRESH_UV_OV.threshVcellOV = 0xff;
    L9963E_DRV_reg_write(
        &(h9l.drv_handle), L9963E_DEVICE_BROADCAST, L9963E_VCELL_THRESH_UV_OV_ADDR, &vcell_thresh_uv_ov_reg, 10);

    // Configuring total voltage tresholds
    L9963E_RegisterUnionTypeDef vbat_sum_th_reg  = {.generic = L9963E_VBATT_SUM_TH_DEFAULT};
    vbat_sum_th_reg.VBATT_SUM_TH.VBATT_SUM_OV_TH = 0xff;
    L9963E_DRV_reg_write(
        &(h9l.drv_handle), L9963E_DEVICE_BROADCAST, L9963E_VBATT_SUM_TH_ADDR, &vbat_sum_th_reg, 10);

    //* BALANCING REGISTER SETTINGS
    L9963E_RegisterUnionTypeDef bal1_reg = {.generic = L9963E_BAL_1_DEFAULT};
    bal1_reg.Bal_1.bal_stop = 1;
    L9963E_DRV_reg_write(
        &(h9l.drv_handle), L9963E_DEVICE_BROADCAST, L9963E_Bal_1_ADDR, &bal1_reg, 10);
    
    L9963E_RegisterUnionTypeDef bal2_reg = {.generic = L9963E_BAL_2_DEFAULT};
    bal2_reg.Bal_2.Balmode = 1;
    L9963E_DRV_reg_write(
        &(h9l.drv_handle), L9963E_DEVICE_BROADCAST, L9963E_Bal_2_ADDR, &bal2_reg, 10);
    
    L9963E_RegisterUnionTypeDef bal3_reg = {.generic = L9963E_BAL_3_DEFAULT};
    L9963E_DRV_reg_write(
        &(h9l.drv_handle), L9963E_DEVICE_BROADCAST, L9963E_Bal_3_ADDR, &bal3_reg, 10);
    
    L9963E_RegisterUnionTypeDef bal4_reg = {.generic = L9963E_BAL_4_DEFAULT};
    L9963E_DRV_reg_write(
        &(h9l.drv_handle), L9963E_DEVICE_BROADCAST, L9963E_Bal_4_ADDR, &bal4_reg, 10);
    
    L9963E_RegisterUnionTypeDef bal5_reg = {.generic = L9963E_BAL_5_DEFAULT};
    L9963E_DRV_reg_write(
        &(h9l.drv_handle), L9963E_DEVICE_BROADCAST, L9963E_Bal_5_ADDR, &bal5_reg, 10);
    
    L9963E_RegisterUnionTypeDef bal6_reg = {.generic = L9963E_BAL_6_DEFAULT};
    L9963E_DRV_reg_write(
        &(h9l.drv_handle), L9963E_DEVICE_BROADCAST, L9963E_Bal_6_ADDR, &bal6_reg, 10);
    
    L9963E_RegisterUnionTypeDef bal7_reg = {.generic = L9963E_BAL_7_DEFAULT};
    L9963E_DRV_reg_write(
        &(h9l.drv_handle), L9963E_DEVICE_BROADCAST, L9963E_Bal_7_ADDR, &bal7_reg, 10);
    
    L9963E_RegisterUnionTypeDef bal8_reg = {.generic = L9963E_BAL_8_DEFAULT};
    L9963E_DRV_reg_write(
        &(h9l.drv_handle), L9963E_DEVICE_BROADCAST, L9963E_Bal_8_ADDR, &bal8_reg, 10);
    
    //abilitazione del regolatore VTREF
    L9963E_enable_vref(&h9l, L9963E_DEVICE_BROADCAST, 1);
    
    //impostazione timeout comunicazione ISOSPI per errori
    L9963E_setCommTimeout(&h9l, _1024MS, L9963E_DEVICE_BROADCAST, 0);
 
    //comunicazione alle board slaves quali celle sono attive
    L9963E_set_enabled_cells(&h9l, L9963E_DEVICE_BROADCAST, ENABLED_CELLS);
}

//* first parameter: module_id in the chain (1 to L9963E_SLAVES_N)
//* second parameter: 1 for read gpsios, 0 for not
void L9963E_utils_read_cells(uint8_t module_id, uint8_t read_gpio) {
  L9963E_StatusTypeDef e;
  uint8_t c_done;
  uint8_t reading_retry_counter = 0;
  do {
    L9963E_poll_conversion(&h9l, module_id, &c_done);
  } while (!c_done);
  L9963E_start_conversion(&h9l, module_id, 0b000, read_gpio ? L9963E_GPIO_CONV : 0);
  
  uint16_t voltage = 0;
  uint32_t voltage32 = 0;
  uint8_t d_rdy = 0;
  uint8_t i = module_id-1;
  for(uint8_t cells_addresses_index = 0; cells_addresses_index < L9963E_CELLS_N; cells_addresses_index++){
    reading_retry_counter = 0;
    do {
      e = L9963E_read_cell_voltage(&h9l, module_id, cells_addresses[cells_addresses_index], &voltage, &d_rdy);
    } while(e != L9963E_OK || !d_rdy || reading_retry_counter++ < MAX_READING_RETRY_L9963E_CELLS);
    if(e == L9963E_OK)
      vcellsm[i][cells_addresses_index] = voltage;
    else
      vcellsm[i][cells_addresses_index] = CELL_VOLTAGE_READ_ERROR;
  }

    /******* READING TOTAL BATTERY VOLTAGES *******/
    //* for each node, read from two registers: one is the sum of all cell voltages, the other is the direct measurement of the total voltage of the single node
    do {
        e = L9963E_read_batt_voltage(&h9l, module_id, ((uint16_t *)&(vtot[module_id-1])), ((uint32_t *)&(vsumbatt[module_id-1])));
    } while (e != L9963E_OK);
  
  /******* READING BATTERY CURRENT *******/
  //? this because the shunt is only on the first node
  if(module_id == 1){
    uint8_t reading_retry_counter = 0;
    do {
      e = L9963E_read_current_mA(&h9l, module_id, L9963E_Ibattery_calib_ADDR, &voltage32);
    } while (e != L9963E_OK ||  reading_retry_counter++ > MAX_READING_RETRY_L9963E_GPIO);
    if(e!=L9963E_OK){
      vbattery_current = 0;
    }else{
      vbattery_current = voltage32;
    }
  }
  
    /******* READING GPIO VOLTAGES *******/
    
    if (!read_gpio)
        return;

    for(uint8_t cell_address_index=0; cell_address_index<L9963E_CELLS_N; cell_address_index++){
      reading_retry_counter = 0;
      do {
        e = L9963E_read_gpio_voltage(&h9l, module_id, gpio_addresses[cell_address_index], &voltage, &d_rdy);
      } while (e != L9963E_OK || !d_rdy || reading_retry_counter++ > MAX_READING_RETRY_L9963E_GPIO);

      if(e== L9963E_OK)
        vgpio[i][cell_address_index] = voltage;
      else
        vgpio[i][cell_address_index] = CELL_TEMPERATURE_READ_ERROR;
    }
}

void L9963E_utils_get_gpios(int16_t* desired_array){
  uint8_t k=0;
  for(uint8_t i=0;i<L9963E_SLAVES_N;i++){
    for(uint8_t j=0;j<L9963E_GPIOS_N;j++){
      desired_array[k]=ntc_get_centideg_from_raw(vcellsm[i][j]);
      ++k;
    }
  }
}

void L9963E_utils_get_cells(uint16_t* desired_array){
  uint8_t k=0;
  for(uint8_t i=0;i<L9963E_SLAVES_N;i++){
    for(uint8_t j=0;j<L9963E_CELLS_N;j++){
      desired_array[k]=L9963E_utils_get_cell_mv(vcellsm[i][j]);
      ++k;
    }
  }
}

uint16_t L9963E_utils_get_cell_mv(uint16_t raw_cell_voltage) {
  return (uint16_t)((float)raw_cell_voltage * 89e-3f);
}

uint32_t L9963E_utils_get_batt_mv() {
  uint32_t vtot_temp = 0;
  //uint32_t vsum_temp = 0;
  for(uint8_t i=0;i<L9963E_SLAVES_N;i++){
    vtot_temp += vtot[i];
    //vsum_temp += vsumbatt[i];
  }
  vtot_temp = (uint32_t)((float)vtot_temp * 1.33f);
  //vsum_temp = (uint32_t)((float)vtot_temp * 89e-3f);
  return vtot_temp;
}

int32_t L9963E_utils_get_batt_mA(){
  return (int32_t)vbattery_current;
}

void balancing_set(uint8_t* cell_to_balance, uint8_t enable){
  L9963E_RegisterUnionTypeDef bal1_conf_reg = {.generic = L9963E_BAL_1_DEFAULT};
  
  if(enable == 0){
    bal1_conf_reg.Bal_1.bal_start = 0;
    bal1_conf_reg.Bal_1.bal_stop  = 1;
    L9963E_DRV_reg_write(&(h9l.drv_handle), L9963E_DEVICE_BROADCAST, L9963E_Bal_1_ADDR, &bal1_conf_reg, 10);
    return;
  }
  L9963E_RegisterUnionTypeDef activate_balancing;
  for(uint8_t i=0;i<L9963E_SLAVES_N;i++){
      activate_balancing.BalCell6_1act.BAL1 = cell_to_balance[(14*i)];
      activate_balancing.BalCell6_1act.BAL2 = cell_to_balance[(14*i + 1)];
      activate_balancing.BalCell6_1act.BAL3 = cell_to_balance[(14*i + 2)];
      activate_balancing.BalCell6_1act.BAL4 = cell_to_balance[(14*i + 3)];
      activate_balancing.BalCell6_1act.BAL5 = cell_to_balance[(14*i + 4)];
      activate_balancing.BalCell6_1act.BAL6 = cell_to_balance[(14*i + 5)];
      L9963E_DRV_reg_write(
        &(h9l.drv_handle), (i+1), L9963E_BalCell6_1act_ADDR, &activate_balancing, 10
      );

      activate_balancing.BalCell14_7act.BAL7 = cell_to_balance[(14*i + 6)];
      activate_balancing.BalCell14_7act.BAL8 = cell_to_balance[(14*i + 7)];
      activate_balancing.BalCell14_7act.BAL9 = cell_to_balance[(14*i + 8)];
      activate_balancing.BalCell14_7act.BAL10 = cell_to_balance[(14*i + 9)];
      activate_balancing.BalCell14_7act.BAL11 = cell_to_balance[(14*i + 10)];
      activate_balancing.BalCell14_7act.BAL12 = cell_to_balance[(14*i + 11)];
      activate_balancing.BalCell14_7act.BAL13 = cell_to_balance[(14*i + 12)];
      activate_balancing.BalCell14_7act.BAL14 = cell_to_balance[(14*i + 13)];
      L9963E_DRV_reg_write(
        &(h9l.drv_handle), (i+1), L9963E_BalCell14_7act_ADDR, &activate_balancing, 10
      );
    }

    //stop and restart balancing
    bal1_conf_reg.Bal_1.bal_start = 0;
    bal1_conf_reg.Bal_1.bal_stop  = 1;
    L9963E_DRV_reg_write(&(h9l.drv_handle), L9963E_DEVICE_BROADCAST, L9963E_Bal_1_ADDR, &bal1_conf_reg, 10);

    bal1_conf_reg.Bal_1.bal_start = 1;
    bal1_conf_reg.Bal_1.bal_stop  = 0;
    L9963E_DRV_reg_write(&(h9l.drv_handle), L9963E_DEVICE_BROADCAST, L9963E_Bal_1_ADDR, &bal1_conf_reg, 10);
}