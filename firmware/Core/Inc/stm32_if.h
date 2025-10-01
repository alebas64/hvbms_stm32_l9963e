/*
 * "THE BEER-WARE LICENSE" (Revision 69):
 * Squadra Corse firmware team wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you 
 * think this stuff is worth it, you can buy us a beer in return.
 * 
 * Authors
 * - Federico Carbone [federico.carbone.sc@gmail.com]
 */

#ifndef STM32_IF_H
#define STM32_IF_H

#include "L9963E_interface.h"

//function for ISOL interface
L9963E_IF_PinState GPIO_ReadPinL(L9963E_IF_PINS pin);
L9963E_StatusTypeDef GPIO_WritePinL(L9963E_IF_PINS pin, L9963E_IF_PinState state);
L9963E_StatusTypeDef SPI_ReceiveL(uint8_t *data, uint8_t size, uint8_t timeout_ms);
L9963E_StatusTypeDef SPI_TransmitL(uint8_t *data, uint8_t size, uint8_t timeout_ms);

//function for ISOH interface
L9963E_IF_PinState GPIO_ReadPinH(L9963E_IF_PINS pin);
L9963E_StatusTypeDef GPIO_WritePinH(L9963E_IF_PINS pin, L9963E_IF_PinState state);
L9963E_StatusTypeDef SPI_ReceiveH(uint8_t *data, uint8_t size, uint8_t timeout_ms);
L9963E_StatusTypeDef SPI_TransmitH(uint8_t *data, uint8_t size, uint8_t timeout_ms);

//common functions for both interfaces
uint32_t GetTickMs(void);
void DelayMs(uint32_t delay);

#endif  //STM32_IF_H