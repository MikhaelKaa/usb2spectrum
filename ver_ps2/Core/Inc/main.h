/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
int mouse_event_cnt;
typedef enum dev_role{
  use_none = 0,
  use_mouse,
  use_kbd
} dev_role_t;
dev_role_t role;

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LD2_Pin GPIO_PIN_13
#define LD2_GPIO_Port GPIOC
#define DV1_Pin GPIO_PIN_0
#define DV1_GPIO_Port GPIOC
#define DV2_Pin GPIO_PIN_1
#define DV2_GPIO_Port GPIOC
#define DV3_Pin GPIO_PIN_2
#define DV3_GPIO_Port GPIOC
#define DV4_Pin GPIO_PIN_3
#define DV4_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define RESET_Pin GPIO_PIN_4
#define RESET_GPIO_Port GPIOC
#define MAGIC_Pin GPIO_PIN_5
#define MAGIC_GPIO_Port GPIOC
#define DV0_Pin GPIO_PIN_0
#define DV0_GPIO_Port GPIOB
#define sega_db9_7_sel_Pin GPIO_PIN_11
#define sega_db9_7_sel_GPIO_Port GPIOB
#define SPI_A0_Pin GPIO_PIN_13
#define SPI_A0_GPIO_Port GPIOB
#define SPI_A1_Pin GPIO_PIN_14
#define SPI_A1_GPIO_Port GPIOB
#define sega_db9_9_Pin GPIO_PIN_6
#define sega_db9_9_GPIO_Port GPIOC
#define sega_db9_6_Pin GPIO_PIN_7
#define sega_db9_6_GPIO_Port GPIOC
#define VBUS_FS_Pin GPIO_PIN_10
#define VBUS_FS_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define sega_db9_4_Pin GPIO_PIN_6
#define sega_db9_4_GPIO_Port GPIOB
#define sega_db9_3_Pin GPIO_PIN_7
#define sega_db9_3_GPIO_Port GPIOB
#define sega_db9_2_Pin GPIO_PIN_8
#define sega_db9_2_GPIO_Port GPIOB
#define sega_db9_1_Pin GPIO_PIN_9
#define sega_db9_1_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
