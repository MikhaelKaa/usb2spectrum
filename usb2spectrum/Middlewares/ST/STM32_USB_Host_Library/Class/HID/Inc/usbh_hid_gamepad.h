/**
  ******************************************************************************
  * @file    usbh_hid_gamepad.h
  * @author  MCD Application Team
  * @brief   This file contains all the prototypes for the usbh_hid_gamepad.c
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                      http://www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive  ----------------------------------------------*/
#ifndef __USBH_HID_gamepad_H
#define __USBH_HID_gamepad_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbh_hid.h"
#include "stdlib.h"

typedef struct _HID_GAMEPAD_Info
{
  uint32_t              data_0;
  uint32_t              data_1;
}
HID_GAMEPAD_Info_TypeDef;

/*      . . . . .                     . . . . . 
      .  _______  .                 .  _______  .
      . |left_2 | .                 . |right_2| .
      .  _______    . . . . . . . .    _______  .
      . |left_1 |      Defender       |right_1| .
    .                                              .
   .    _             clear                (1)       .
  .   _| |_     turbo   O    auto                      .
  .  |_   _|      O           O      (4)         (2)   .
   .   |_|                                            .
    .               =       >              (3)       .
      .           stop(9)   play(10)              .
          .    .    .   .    .     .    .     .
                      with autofire
*/
typedef struct GAMEPAD_Keys0
{
  uint8_t  data_nc0;
  uint8_t  data_nc1;
  uint8_t  data_nc2;
  uint8_t  axis_x;

  uint8_t  axis_y;

  unsigned  data_nc3  :1;
  unsigned  data_nc4  :1;
  unsigned  data_nc5  :1;
  unsigned  data_nc6  :1;

  unsigned  button_1  :1;
  unsigned  button_2  :1;
  unsigned  button_3  :1;
  unsigned  button_4  :1;

  unsigned  left_1  :1;
  unsigned  right_1 :1;
  unsigned  left_2  :1;
  unsigned  right_2 :1;

  unsigned  button_stop:1;
  unsigned  button_play:1;
  unsigned  button_nc0:1;
  unsigned  button_nc1:1;

  uint8_t  data_nc7;
}
GAMEPAD_Keys_TypeDef0;

typedef struct GAMEPAD_Keys1
{
  uint8_t  axis_x;
  uint8_t  axis_y;
  uint8_t  data_nc0;
  uint8_t  data_nc1;
  uint8_t  data_nc2;


  unsigned  data_nc3  :1;
  unsigned  data_nc4  :1;
  unsigned  data_nc5  :1;
  unsigned  data_nc6  :1;

  unsigned  button_1  :1;
  unsigned  button_2  :1;
  unsigned  button_3  :1;
  unsigned  button_4  :1;

  unsigned  left_1  :1;
  unsigned  right_1 :1;
  unsigned  left_2  :1;
  unsigned  right_2 :1;

  unsigned  button_stop:1;
  unsigned  button_play:1;
  unsigned  button_nc0:1;
  unsigned  button_nc1:1;

  uint8_t  data_nc7;
}
GAMEPAD_Keys_TypeDef1;

/** @addtogroup USBH_LIB
  * @{
  */

/** @addtogroup USBH_CLASS
  * @{
  */

/** @addtogroup USBH_HID_CLASS
  * @{
  */

/** @defgroup USBH_HID_gamepad
  * @brief This file is the Header file for usbh_hid_gamepad.c
  * @{
  */


/** @defgroup USBH_HID_gamepad_Exported_Types
  * @{
  */

/**
  * @}
  */

/** @defgroup USBH_HID_gamepad_Exported_Defines
  * @{
  */
/**
  * @}
  */

/** @defgroup USBH_HID_gamepad_Exported_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup USBH_HID_gamepad_Exported_Variables
  * @{
  */
/**
  * @}
  */

/** @defgroup USBH_HID_gamepad_Exported_FunctionsPrototype
  * @{
  */
USBH_StatusTypeDef USBH_HID_GamepadInit(USBH_HandleTypeDef *phost);
uint8_t *USBH_HID_GetGamepadInfo(USBH_HandleTypeDef *phost);
static uint16_t collect_bits(uint8_t *p, uint16_t offset, uint8_t size, int is_signed);
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __USBH_HID_gamepad_H */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
