/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, L9963T_H_NCS_GPIO_OUT_Pin|L9963T_L_NCS_GPIO_OUT_Pin|L9963T_L_DIS_GPIO_INOUT_Pin|L9963T_H_BNE_GPIO_IN_Pin
                          |L9963T_H_DIS_GPIO_INOUT_Pin|OBC_HV_POSITIVE_CONTACTOR_Pin|DCDC_HV_POSITIVE_RELE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, L9963T_L_ISOFREQ_GPIO_OUT_Pin|L9963T_L_TXEN_GPIO_OUT_Pin|BATTERY_HV_POSITIVE_CONTACTOR_Pin|BATTERY_HV_NEGATIVE_CONTACTOR_Pin
                          |BATTERY_HV_PRECHARGE_CONTACTOR_Pin|BATTERY_HV_DISCHARGE_CONTACTOR_Pin|L9963T_H_ISOFREQ_GPIO_OUT_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(MPPT_HV_PRECHARGE_RELE_GPIO_Port, MPPT_HV_PRECHARGE_RELE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(MPPT_HV_POSITIVE_CONTACTOR_GPIO_Port, MPPT_HV_POSITIVE_CONTACTOR_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : L9963T_H_NCS_GPIO_OUT_Pin L9963T_L_NCS_GPIO_OUT_Pin L9963T_L_DIS_GPIO_INOUT_Pin L9963T_H_BNE_GPIO_IN_Pin
                           L9963T_H_DIS_GPIO_INOUT_Pin OBC_HV_POSITIVE_CONTACTOR_Pin DCDC_HV_POSITIVE_RELE_Pin */
  GPIO_InitStruct.Pin = L9963T_H_NCS_GPIO_OUT_Pin|L9963T_L_NCS_GPIO_OUT_Pin|L9963T_L_DIS_GPIO_INOUT_Pin|L9963T_H_BNE_GPIO_IN_Pin
                          |L9963T_H_DIS_GPIO_INOUT_Pin|OBC_HV_POSITIVE_CONTACTOR_Pin|DCDC_HV_POSITIVE_RELE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : L9963T_L_ISOFREQ_GPIO_OUT_Pin L9963T_L_TXEN_GPIO_OUT_Pin BATTERY_HV_POSITIVE_CONTACTOR_Pin BATTERY_HV_NEGATIVE_CONTACTOR_Pin
                           BATTERY_HV_PRECHARGE_CONTACTOR_Pin BATTERY_HV_DISCHARGE_CONTACTOR_Pin L9963T_H_ISOFREQ_GPIO_OUT_Pin */
  GPIO_InitStruct.Pin = L9963T_L_ISOFREQ_GPIO_OUT_Pin|L9963T_L_TXEN_GPIO_OUT_Pin|BATTERY_HV_POSITIVE_CONTACTOR_Pin|BATTERY_HV_NEGATIVE_CONTACTOR_Pin
                          |BATTERY_HV_PRECHARGE_CONTACTOR_Pin|BATTERY_HV_DISCHARGE_CONTACTOR_Pin|L9963T_H_ISOFREQ_GPIO_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : L9963T_L_BNE_GPIO_IN_Pin */
  GPIO_InitStruct.Pin = L9963T_L_BNE_GPIO_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(L9963T_L_BNE_GPIO_IN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : L9963T_H_TXEN_GPIO_OUT_Pin */
  GPIO_InitStruct.Pin = L9963T_H_TXEN_GPIO_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(L9963T_H_TXEN_GPIO_OUT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : FAULT_L9963E_H_Pin FAULT_L9963E_L_Pin */
  GPIO_InitStruct.Pin = FAULT_L9963E_H_Pin|FAULT_L9963E_L_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : MPPT_HV_PRECHARGE_RELE_Pin */
  GPIO_InitStruct.Pin = MPPT_HV_PRECHARGE_RELE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(MPPT_HV_PRECHARGE_RELE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : MPPT_HV_POSITIVE_CONTACTOR_Pin */
  GPIO_InitStruct.Pin = MPPT_HV_POSITIVE_CONTACTOR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(MPPT_HV_POSITIVE_CONTACTOR_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
