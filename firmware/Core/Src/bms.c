#include "bms.h"
uint8_t currentstate = HV_OFF;

uint8_t battery_is_safe = 0;

pack_information_t *pack_information;
battery_errors_t   *battery_errors;
hv_devices_states_requests_t *hv_devices_states_requests;

//* setup function for the bms routine
void bms_routines_init(pack_information_t *ptr1, battery_errors_t *ptr2, hv_devices_states_requests_t *ptr3){
    //avvio pwm per ventole
    TIMER_FAN_PWM->CCR1 = 0;
    HAL_TIM_PWM_Start_IT(&htim12,TIM_CHANNEL_1);

    for(uint8_t i=0;i<L9963Es_TOTAL_CELLS_N;i++){
        pack_information->cell_voltages_mV[i] = CELL_VOLTAGE_READ_ERROR;
        pack_information->cell_balancing[i] = CELL_BALANCING_OFF;
    }

    for(uint8_t i=0;i<L9963Es_TOTAL_GPIOS_N;i++){
        pack_information->cell_temperatures_centideg[i] = CELL_TEMPERATURE_READ_ERROR;
    }

    pack_information = ptr1;
    battery_errors = ptr2;
    hv_devices_states_requests = ptr3;
}

void bms_routine(){
    #ifdef SERIAL_DEBUG
    uint8_t buf[50];
    strcpy((char*)buf,"sono dentro la bmsroutine\r\n");
    HAL_UART_Transmit(&huart2,buf,strlen((char*)buf),HAL_MAX_DELAY);
    #endif

    for(uint8_t n_modules=1;n_modules<=L9963E_SLAVES_N;n_modules++)
        L9963E_utils_read_cells(n_modules,1);
    
    #ifdef SERIAL_DEBUG
    strcpy((char*)buf,"finita lettura\r\n");
    HAL_UART_Transmit(&huart2,buf,strlen((char*)buf),HAL_MAX_DELAY);
    #endif

    pack_information->pack_voltage_mV = L9963E_utils_get_batt_mv();
    L9963E_utils_get_cells(pack_information->cell_voltages_mV);
    L9963E_utils_get_gpios(pack_information->cell_temperatures_centideg);
    pack_information->current_mA = L9963E_utils_get_batt_mA();

    #ifdef SERIAL_DEBUG
    for(uint8_t a=0;a<L9963Es_TOTAL_CELLS_N;a++){
        sprintf(buf,"cella %2d: %x %x\r\n",a,((pack_information->cell_voltages_mV[a] & 0xFF00)>8),(pack_information->cell_voltages_mV[a] & 0xFF));
        HAL_UART_Transmit(&huart2,buf,strlen((char*)buf),HAL_MAX_DELAY);
    }
    #endif

    //* lettura di tutti i parametri della batteria
    battery_is_safe = battery_operability();
    compute_max_deltas();

    if(!battery_is_safe){
        hv_output_change_state(HV_BRUTAL_OFF);
        currentstate=HV_BRUTAL_OFF;
    }

    switch(currentstate){
        case HV_OFF:
            //* vedere se possibile passare in HV_ON
            solar_charging_procedure(SOLAR_CHARGING_STOP);
            powertrain_control(POWERTRAIN_STOP);
            vac_charging_procedure(VAC_CHARGING_STOP);
            if(battery_is_safe){
                currentstate = HV_ON;
            }
            break;
        
        case HV_PRECHARGE:
            solar_charging_procedure(SOLAR_CHARGING_STOP);
            powertrain_control(POWERTRAIN_STOP);
            vac_charging_procedure(VAC_CHARGING_STOP);
            currentstate = HV_ON;
            break;

        case HV_ON:
            //* normale utilizzo. Fare tutti i controlli
            solar_charging_procedure(SOLAR_CHARGING_START);
            powertrain_control(POWERTRAIN_START);
            vac_charging_procedure(VAC_CHARGING_STOP);
            break;

        case VAC_CHARGING:
            solar_charging_procedure(SOLAR_CHARGING_STOP);
            powertrain_control(POWERTRAIN_STOP);
            vac_charging_procedure(VAC_CHARGING_START);
            break;

        default:
            //! invalid state, something went wrong
            break;
    }

    //* these procedure are always executed, regardless of the state
    balancing_procedure();
    temperature_control();
}

//in base alla temperatura delle celle viene modificata la velocità delle ventole
void temperature_control(){
    uint8_t newduty;
    int16_t temperature_for_fan_control;
    if(pack_information->cell_delta_temperature_centideg > IGNORE_AVG_TEMPERATURE_DELTA_THRESHOLD){
        temperature_for_fan_control = pack_information->cell_max_temperature_centideg;
    }else{
        temperature_for_fan_control = pack_information->cell_temperature_avg_centideg;
    }
    //se la differenza tra max e min è troppo alta, usare la max
    if(temperature_for_fan_control < T_LOW){
        newduty = FAN_OFF;
    }else if(temperature_for_fan_control < T_MEDIUM){
        newduty = FAN_LOW;
    }else if(temperature_for_fan_control < T_HIGH){
        newduty = FAN_MEDIUM;
    }else if(temperature_for_fan_control < T_MAX){
        newduty = FAN_HIGH;
    }else{
        newduty = FAN_MAX;
    }
    
    battery_fan_duty(newduty);
}

//controlla la tensione delle celle e imposta il balancing (deve controllare anche la corrente?)
void balancing_procedure(){
    if(pack_information->cell_max_voltage_mV > HARD_OVERVOLT){
        for(uint8_t i=0;i<L9963Es_TOTAL_CELLS_N;i++){
            if(pack_information->cell_voltages_mV[i] > HARD_OVERVOLT){
                pack_information->cell_balancing[i] = CELL_BALANCING_ON;
                pack_information->cell_balancing_number++;
            }else{
                pack_information->cell_balancing[i] = CELL_BALANCING_OFF;
            }
        }
    }else{
        //* if at least one cell is above DEF_HIGH_V_START_mV the balancing procedure can start
        if(pack_information->battery_state.is_charging == 0){
            return; //* no balancing while not charging
        }

        //* the battery is charging, check if balancing conditions are met
        uint8_t any_above_high = 0;
        for (uint8_t i = 0; i < L9963Es_TOTAL_CELLS_N; i++) {
            if (pack_information->cell_voltages_mV[i] > DEF_HIGH_V_START_mV) {
                any_above_high = 1;
                break;
            }
        }

        //* these balancing conditions needs to be true for balancing to start 
        if (!(any_above_high==1) &&
            !(pack_information->pack_cell_delta_mV > DEF_DELTA_START_mV)    ){
            return; //* no balancing needed
        }

        //* start balancing procedure
        pack_information->cell_balancing_number = 0;
        for (uint8_t i = 0; i < L9963Es_TOTAL_CELLS_N; i++) {
            if (pack_information->cell_voltages_mV[i] >= DEF_HIGH_V_START_mV) {
                //* the cell could be balanced
                if((int16_t)(pack_information->cell_voltages_mV[i] - (int16_t)(pack_information->cell_min_voltage_mV)) > DEF_BALANCE_BAND_mV){
                    pack_information->cell_balancing[i] = CELL_BALANCING_ON; /* è consentito bilanciare più celle contemporaneamente */
                    pack_information->cell_balancing_number++;
                }else{
                    pack_information->cell_balancing[i] = CELL_BALANCING_OFF;
                }
            }else{
                pack_information->cell_balancing[i] = CELL_BALANCING_OFF;
            }
        }
    }
    //* function for setting the balancing on the L9963Es
    balancing_set(pack_information->cell_balancing, pack_information->cell_balancing_number > 0 ? 1 : 0);
}

/* compute max, min and delta of all cell voltages */
void compute_max_deltas(){
    uint16_t min_out = max_search(pack_information->cell_voltages_mV, L9963Es_TOTAL_CELLS_N);
    uint16_t max_out = min_search(pack_information->cell_voltages_mV, L9963Es_TOTAL_CELLS_N);
    pack_information->cell_min_voltage_mV = min_out;
    pack_information->cell_max_voltage_mV = max_out;
    pack_information->pack_cell_delta_mV = max_out - min_out;
    pack_information->cell_voltage_avg_mV = get_avg(pack_information->cell_voltages_mV, L9963Es_TOTAL_CELLS_N);

    int16_t min_out_signed = max_search_signed(pack_information->cell_temperatures_centideg, L9963Es_TOTAL_GPIOS_N);
    int16_t max_out_signed = min_search_signed(pack_information->cell_temperatures_centideg, L9963Es_TOTAL_GPIOS_N);
    pack_information->cell_min_temperature_centideg = min_out_signed;
    pack_information->cell_max_temperature_centideg = max_out_signed;
    pack_information->cell_delta_temperature_centideg = max_out_signed - min_out_signed;
    pack_information->cell_temperature_avg_centideg = get_avg_signed(pack_information->cell_temperatures_centideg, L9963Es_TOTAL_GPIOS_N);
}

//controlla se la batteria è sicura da utilizzare. 0 per no, 1 per si
uint8_t battery_operability(){
    uint8_t error_counter = 0; //se questo valore rimane a 0 allora la batteria è utilizzabile
    //controllare questi prototipi, succede un out of index
    error_counter += voltage_check();
    error_counter += current_check();
    error_counter += temperature_check();
    return error_counter == ERROR_NOT_PRESENT ? 1 : 0;
}

//* if 0 is returned the voltages are okay, if not the voltages are not okay
uint8_t voltage_check(){
    uint8_t status = 0;

    //* check if the voltage pack is okay
    if(pack_information->pack_voltage_mV > PACK_OVERVOLTAGE){
        status++;
        battery_errors->pack_overvoltage = ERROR_PRESENT;
    }else{
        battery_errors->pack_overvoltage = ERROR_NOT_PRESENT;
        if(pack_information->pack_voltage_mV < PACK_UNDERVOLTAGE){
            status++;
            battery_errors->pack_undervoltage = ERROR_PRESENT;
        }else{
            battery_errors->pack_undervoltage = ERROR_NOT_PRESENT;
        }
    }

    //* check if the cell voltages are okay
    for(uint8_t i=0; i<L9963Es_TOTAL_CELLS_N; i++) {
        if(pack_information->cell_voltages_mV[i] > SOFT_OVERVOLT){
            if(pack_information->cell_voltages_mV[i] > HARD_OVERVOLT){
                status++;
                battery_errors->cell_soft_overvoltage[i] = ERROR_NOT_PRESENT;
                battery_errors->cell_hard_overvoltage[i] = ERROR_PRESENT;
            }else{
                battery_errors->cell_soft_overvoltage[i] = ERROR_PRESENT;
                battery_errors->cell_hard_overvoltage[i] = ERROR_NOT_PRESENT;
            }
        }else{
            battery_errors->cell_soft_overvoltage[i] = ERROR_NOT_PRESENT;
            battery_errors->cell_hard_overvoltage[i] = ERROR_NOT_PRESENT;
        }

        if(pack_information->cell_voltages_mV[i] < SOFT_UNDERVOLT){
            if(pack_information->cell_voltages_mV[i] < HARD_UNDERVOLT){
                status++;
                battery_errors->cell_soft_undervoltage[i] = ERROR_NOT_PRESENT;
                battery_errors->cell_hard_undervoltage[i] = ERROR_PRESENT;
            }else{
                battery_errors->cell_soft_undervoltage[i] = ERROR_PRESENT;
                battery_errors->cell_hard_undervoltage[i] = ERROR_NOT_PRESENT;
            }
        }else{
            battery_errors->cell_soft_undervoltage[i] = ERROR_NOT_PRESENT;
            battery_errors->cell_hard_undervoltage[i] = ERROR_NOT_PRESENT;
        }
    }
    return status;
}

//le funzioni richiamate per set e reset non sono ancora state scritte (nemmeno i prototipi)
uint8_t temperature_check(){
    uint8_t status = 0;
    for(uint8_t i=0; i<L9963Es_TOTAL_CELLS_N; i++) {
        if(pack_information->cell_temperatures_centideg[i] > SOFT_OVERTEMP){
            if(pack_information->cell_temperatures_centideg[i] > HARD_OVERTEMP){
                status++;
                battery_errors->cell_soft_overtemp[i] = ERROR_NOT_PRESENT;
                battery_errors->cell_hard_overtemp[i] = ERROR_PRESENT;
            }else{
                battery_errors->cell_soft_overtemp[i] = ERROR_PRESENT;
                battery_errors->cell_hard_overtemp[i] = ERROR_NOT_PRESENT;
            }
        }else{
            battery_errors->cell_soft_overtemp[i] = ERROR_NOT_PRESENT;
            battery_errors->cell_hard_overtemp[i] = ERROR_NOT_PRESENT;
        }

        if(pack_information->cell_temperatures_centideg[i] < SOFT_UNDERTEMP){
            if(pack_information->cell_temperatures_centideg[i] < HARD_UNDERTEMP){
                status++;
                battery_errors->cell_soft_undertemp[i] = ERROR_NOT_PRESENT;
                battery_errors->cell_hard_undertemp[i] = ERROR_PRESENT;
            }else{
                battery_errors->cell_soft_undertemp[i] = ERROR_PRESENT;
                battery_errors->cell_hard_undertemp[i] = ERROR_NOT_PRESENT;
            }
        }else{
            battery_errors->cell_soft_undertemp[i] = ERROR_NOT_PRESENT;
            battery_errors->cell_hard_undertemp[i] = ERROR_NOT_PRESENT;
        }
    }
    return status;
}

uint8_t current_check(){
    if(pack_information->current_mA > MIN_CURRENT_FOR_CHARGING_STATE_mA){
        pack_information->battery_state.is_charging = 1;
        pack_information->battery_state.is_discharging = 0;
    }
    else if((-pack_information->current_mA) > MIN_CURRENT_FOR_DISCHARGING_STATE_mA){
        pack_information->battery_state.is_charging = 0;
        pack_information->battery_state.is_discharging = 1;
    }

    uint8_t status = 0;
    if(pack_information->current_mA > 0){
        if((pack_information->current_mA) > MAX_CHARGE_CURRENT){
            status++;
            battery_errors->battery_overcurrent_charge = ERROR_PRESENT;
        }else{
            battery_errors->battery_overcurrent_charge = ERROR_NOT_PRESENT;
        }
    }else{
        if(-(pack_information->current_mA) > MAX_DISCHARGE_CURRENT){
            status++;
            battery_errors->battery_overcurrent_discharge = ERROR_PRESENT;
        }else{
            battery_errors->battery_overcurrent_discharge = ERROR_NOT_PRESENT;
        }
    }

    return status;
}

void hv_output_change_state(uint8_t newstate){

}

//funzione per aggiustare facilmente il duty cycle delle ventole per il raffreddamento
void battery_fan_duty(uint8_t duty){
    if(duty>FAN_MAX_DUTY){
        TIMER_FAN_PWM->CCR1=FAN_MAX_DUTY;
        return;
    }
    
    if(duty<FAN_MIN_DUTY){
        TIMER_FAN_PWM->CCR1 = FAN_MIN_DUTY;
        return;  
    }

    TIMER_FAN_PWM->CCR1=duty;
}

void gpiowrite(uint8_t pin, uint8_t state){
    switch(pin){

        default:
            break;
    }
}

//*-------- communication with other HV devices ------------
//controlla il comportamento del pacco batteria durante la ricarica da presa type2

uint32_t charger_millis;
uint8_t charging_first_time = 0;
void vac_charging_procedure(uint8_t command_request){
    if(VAC_CHARGING_STOP){
        charging_first_time = 1;
        hv_devices_states_requests->obc.charging_voltage = 0;
        hv_devices_states_requests->obc.charging_current = 0;
        hv_devices_states_requests->obc.charging_current_state = CHARGING_STATE_A;
    }else if(VAC_CHARGING_START){
        //*calculation for current avaiable via type2standard
        //!REDACTED
        //*charging procedure
        if(pack_information->battery_state.is_charging_allowed == 1 && hv_devices_states_requests->obc.charging_avaiable_states == CHARGING_STATE_C){
            if(charging_first_time == 1){
                charging_first_time = 0;
                charger_millis = HAL_GetTick();
                hv_devices_states_requests->obc.charging_voltage = PACK_CHARGING_VOLTAGE_mV;
                hv_devices_states_requests->obc.charging_current = PACK_CHARGING_CURRENT_START_mA;
                hv_devices_states_requests->obc.charging_current_state = CHARGING_STATE_C;
            }else{
                //gradually increase charging current
                if(timer_check(PACK_CHARGING_TIME_RISE_WAIT_ms,charger_millis)){
                    charger_millis = HAL_GetTick();
                    if(hv_devices_states_requests->obc.charging_current + PACK_CHARGING_CURRENT_RISE_STEP_mA < PACK_CHARGING_CURRENT_mA){
                        hv_devices_states_requests->obc.charging_current = PACK_CHARGING_CURRENT_START_mA + PACK_CHARGING_CURRENT_RISE_STEP_mA;
                    }else{
                        hv_devices_states_requests->obc.charging_current = PACK_CHARGING_CURRENT_mA;
                    }
                }
                if(pack_information->pack_voltage_mV == PACK_CHARGING_VOLTAGE_mV){
                    //CV charging phase - check when charging need to be stopped
                    if(pack_information->current_mA <= PACK_CHARGING_CURRENT_END_mA){
                        pack_information->battery_state.is_charging_allowed = 0;
                        charging_first_time = 1;
                        hv_devices_states_requests->obc.charging_voltage = 0;
                        hv_devices_states_requests->obc.charging_current = 0;
                        hv_devices_states_requests->obc.charging_current_state = CHARGING_STATE_A;
                    }
                }//else we are in CC charging phase
            }
        }
    }
}

//controlla la ricarica tramite i pannelli fotovoltaici
void solar_charging_procedure(uint8_t command_request){
    //!REDACTED
}

//comunica alla centralina del powertrain cosa è possibile fare
//(potenza erogabile e quantità regen permessa)
void powertrain_control(uint8_t command_request){
    //!REDACTED
}

pack_information_t* get_pack_information(){
    return pack_information;
}