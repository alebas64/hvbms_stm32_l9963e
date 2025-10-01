/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define L9963T_H_NCS_GPIO_OUT_Pin GPIO_PIN_0
#define L9963T_H_NCS_GPIO_OUT_GPIO_Port GPIOC
#define L9963T_H_MISO_SDO_Pin GPIO_PIN_1
#define L9963T_H_MISO_SDO_GPIO_Port GPIOC
#define L9963T_H_MOSI_SDI_Pin GPIO_PIN_2
#define L9963T_H_MOSI_SDI_GPIO_Port GPIOC
#define L9963T_L_SCK_Pin GPIO_PIN_5
#define L9963T_L_SCK_GPIO_Port GPIOA
#define L9963T_L_MISO_SDO_Pin GPIO_PIN_6
#define L9963T_L_MISO_SDO_GPIO_Port GPIOA
#define L9963T_L_MOSI_SDI_Pin GPIO_PIN_7
#define L9963T_L_MOSI_SDI_GPIO_Port GPIOA
#define L9963T_L_NCS_GPIO_OUT_Pin GPIO_PIN_4
#define L9963T_L_NCS_GPIO_OUT_GPIO_Port GPIOC
#define L9963T_L_DIS_GPIO_INOUT_Pin GPIO_PIN_5
#define L9963T_L_DIS_GPIO_INOUT_GPIO_Port GPIOC
#define L9963T_L_ISOFREQ_GPIO_OUT_Pin GPIO_PIN_0
#define L9963T_L_ISOFREQ_GPIO_OUT_GPIO_Port GPIOB
#define L9963T_L_BNE_GPIO_IN_Pin GPIO_PIN_1
#define L9963T_L_BNE_GPIO_IN_GPIO_Port GPIOB
#define L9963T_L_TXEN_GPIO_OUT_Pin GPIO_PIN_2
#define L9963T_L_TXEN_GPIO_OUT_GPIO_Port GPIOB
#define L9963T_H_SCK_Pin GPIO_PIN_10
#define L9963T_H_SCK_GPIO_Port GPIOB
#define BATTERY_COOLING_FANS_Pin GPIO_PIN_14
#define BATTERY_COOLING_FANS_GPIO_Port GPIOB
#define L9963T_H_BNE_GPIO_IN_Pin GPIO_PIN_6
#define L9963T_H_BNE_GPIO_IN_GPIO_Port GPIOC
#define L9963T_H_TXEN_GPIO_OUT_Pin GPIO_PIN_7
#define L9963T_H_TXEN_GPIO_OUT_GPIO_Port GPIOC
#define L9963T_H_DIS_GPIO_INOUT_Pin GPIO_PIN_9
#define L9963T_H_DIS_GPIO_INOUT_GPIO_Port GPIOC
#define FAULT_L9963E_H_Pin GPIO_PIN_9
#define FAULT_L9963E_H_GPIO_Port GPIOA
#define FAULT_L9963E_L_Pin GPIO_PIN_10
#define FAULT_L9963E_L_GPIO_Port GPIOA
#define MPPT_HV_PRECHARGE_RELE_Pin GPIO_PIN_15
#define MPPT_HV_PRECHARGE_RELE_GPIO_Port GPIOA
#define OBC_HV_POSITIVE_CONTACTOR_Pin GPIO_PIN_11
#define OBC_HV_POSITIVE_CONTACTOR_GPIO_Port GPIOC
#define DCDC_HV_POSITIVE_RELE_Pin GPIO_PIN_12
#define DCDC_HV_POSITIVE_RELE_GPIO_Port GPIOC
#define MPPT_HV_POSITIVE_CONTACTOR_Pin GPIO_PIN_2
#define MPPT_HV_POSITIVE_CONTACTOR_GPIO_Port GPIOD
#define BATTERY_HV_POSITIVE_CONTACTOR_Pin GPIO_PIN_3
#define BATTERY_HV_POSITIVE_CONTACTOR_GPIO_Port GPIOB
#define BATTERY_HV_NEGATIVE_CONTACTOR_Pin GPIO_PIN_4
#define BATTERY_HV_NEGATIVE_CONTACTOR_GPIO_Port GPIOB
#define BATTERY_HV_PRECHARGE_CONTACTOR_Pin GPIO_PIN_5
#define BATTERY_HV_PRECHARGE_CONTACTOR_GPIO_Port GPIOB
#define BATTERY_HV_DISCHARGE_CONTACTOR_Pin GPIO_PIN_6
#define BATTERY_HV_DISCHARGE_CONTACTOR_GPIO_Port GPIOB
#define L9963T_H_ISOFREQ_GPIO_OUT_Pin GPIO_PIN_8
#define L9963T_H_ISOFREQ_GPIO_OUT_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
/*
     /\_____/\
    /  o   o  \
   ( ==  ^  == )
    )         (
   (           )
  ( (  )   (  ) )
 (__(__)___(__)__)
*/

#define CAN_SEND_TIMER 1
#define CAN_RECEIVE_TIMER 2
#define BMS_ROUTINE_TIMER 3

#define TIMER_CAN_SEND TIM7
#define TIMER_CAN_RECEIVE TIM13
#define TIMER_BMS_LOGIC TIM6
#define TIMER_FAN_PWM TIM12

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
