/**
  ******************************************************************************
  * @file    usbh_hid_gamepad.c
  * @author  MCD Application Team
  * @brief   This file is the application layer for USB Host HID gamepad Handling.
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

  /* BSPDependencies
  - "stm32xxxxx_{eval}{discovery}{nucleo_144}.c"
  - "stm32xxxxx_{eval}{discovery}_io.c"
  - "stm32xxxxx_{eval}{discovery}{adafruit}_lcd.c"
  - "stm32xxxxx_{eval}{discovery}_sdram.c"
  EndBSPDependencies */

/* Includes ------------------------------------------------------------------*/

#include "usbh_hid_parser.h"
#include "usbh_hid_gamepad.h"
#include <stdio.h>
#include <stdlib.h>


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
  * @brief    This file includes HID Layer Handlers for USB Host HID class.
  * @{
  */

/** @defgroup USBH_HID_gamepad_Private_TypesDefinitions
  * @{
  */
/**
  * @}
  */


/** @defgroup USBH_HID_gamepad_Private_Defines
  * @{
  */
/**
  * @}
  */


/** @defgroup USBH_HID_gamepad_Private_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup USBH_HID_gamepad_Private_FunctionPrototypes
  * @{
  */
#define JOYSTICK_AXIS_MIN           0
#define JOYSTICK_AXIS_MID           127
#define JOYSTICK_AXIS_MAX           255
#define JOYSTICK_AXIS_TRIGGER_MIN   64
#define JOYSTICK_AXIS_TRIGGER_MAX   192

#define JOY_RIGHT       0x01
#define JOY_LEFT        0x02
#define JOY_DOWN        0x04
#define JOY_UP          0x08
#define JOY_BTN_SHIFT   4
#define JOY_BTN1        0x10
#define JOY_BTN2        0x20
#define JOY_BTN3        0x40
#define JOY_BTN4        0x80
#define JOY_MOVE        (JOY_RIGHT|JOY_LEFT|JOY_UP|JOY_DOWN)

static USBH_StatusTypeDef USBH_HID_GamepadDecode(USBH_HandleTypeDef *phost);

/**
  * @}
  */


/** @defgroup USBH_HID_gamepad_Private_Variables
  * @{
  */
HID_HandleTypeDef       gamepad_info;
uint32_t                gamepad_report_data[2];
uint32_t                gamepad_rx_report_buf[2];
HID_GAMEPAD_Info_TypeDef  gamepad_data;

static uint16_t collect_bits(uint8_t *p, uint16_t offset, uint8_t size, int is_signed) {
  // mask unused bits of first byte
  uint8_t mask = 0xff << (offset&7);
  uint8_t byte = offset/8;
  uint8_t bits = size;
  uint8_t shift = offset&7;

  uint16_t rval = (p[byte++] & mask) >> shift;
  mask = 0xff;
  shift = 8-shift;
  bits -= shift;

  // first byte already contained more bits than we need
  if(shift > size) {
    // mask unused bits
    rval &= (1<<size)-1;
  } else {
    // further bytes if required
    while(bits) {
      mask = (bits<8)?(0xff>>(8-bits)):0xff;
      rval += (p[byte++] & mask) << shift;
      shift += 8;
      bits -= (bits>8)?8:bits;
    }
  }

  if(is_signed) {
    // do sign expansion
    uint16_t sign_bit = 1<<(size-1);
    if(rval & sign_bit) {
      while(sign_bit) {
	rval |= sign_bit;
	sign_bit <<= 1;
      }

    }
  }

  return rval;
}

/**
  * @}
  */

/** @defgroup USBH_HID_gamepad_Private_Functions
  * @{
  */

/**
  * @brief  USBH_HID_gamepadInit
  *         The function init the HID gamepad.
  * @param  phost: Host handle
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_HID_GamepadInit(USBH_HandleTypeDef *phost)
{
  uint32_t x;
  HID_HandleTypeDef *HID_Handle = (HID_HandleTypeDef *) phost->pActiveClass->pData;
  
  for (x = 0U; x < (sizeof(gamepad_report_data) / sizeof(uint32_t)); x++)
  {
    gamepad_report_data[x] = 0U;
    gamepad_rx_report_buf[x] = 0U;
  }
  
  if (HID_Handle->length > (sizeof(gamepad_report_data)))
  {
    HID_Handle->length = (sizeof(gamepad_report_data));
  }

  HID_Handle->pData = (uint8_t *)(void *)gamepad_rx_report_buf;
  USBH_HID_FifoInit(&HID_Handle->fifo, phost->device.Data, HID_QUEUE_SIZE * sizeof(gamepad_report_data));

  return USBH_OK;
}

/**
  * @brief  USBH_HID_GetgamepadInfo
  *         The function return gamepad information.
  * @param  phost: Host handle
  * @retval gamepad information
  */
uint8_t *USBH_HID_GetGamepadInfo(USBH_HandleTypeDef *phost)
{
    //refresh value of joymap and return value
    if(USBH_HID_GamepadDecode(phost)== USBH_OK)
    {
        return &gamepad_data;
    }
    else
    {
        return NULL;
    }
}

/**
  * @brief  USBH_HID_gamepadDecode
  *         The function decode gamepad data.
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef USBH_HID_GamepadDecode(USBH_HandleTypeDef *phost)
{
    HID_HandleTypeDef *HID_Handle = (HID_HandleTypeDef *) phost->pActiveClass->pData;

    if(HID_Handle->length == 0U)
    {
        return USBH_FAIL;
    }

    if(USBH_HID_FifoRead(&HID_Handle->fifo, &gamepad_report_data, HID_Handle->length) ==  HID_Handle->length)
    {
    
        //USBH_UsrLog("%08X %08X\n", gamepad_report_data[0], gamepad_report_data[1]);
        gamepad_data.data_0 = gamepad_report_data[0];
        gamepad_data.data_1 = gamepad_report_data[1];

        uint8_t up_down = *(((uint8_t*)(&gamepad_report_data[1])) + 0);
        uint8_t l_r = *(((uint8_t*)(&gamepad_report_data[0])) + 3);

        if(up_down < 127) {gamepad_data.up = 1; gamepad_data.down = 0;}
        if(up_down > 127) {gamepad_data.up = 0; gamepad_data.down = 1;}
        if(up_down == 127) {gamepad_data.up = 0; gamepad_data.down = 0;}
        
        if(l_r < 127) {gamepad_data.left = 1; gamepad_data.right = 0;}
        if(l_r > 127) {gamepad_data.left = 0; gamepad_data.right = 1;}
        if(l_r == 127) {gamepad_data.left = 0; gamepad_data.right = 0;}
        
        gamepad_data.b1 = ((gamepad_report_data[1] & 0x0000F000) >> (3*4 + 0) ) & 1;
        gamepad_data.b2 = ((gamepad_report_data[1] & 0x0000F000) >> (3*4 + 1) ) & 1;
        gamepad_data.b3 = ((gamepad_report_data[1] & 0x0000F000) >> (3*4 + 2) ) & 1;
        gamepad_data.b4 = ((gamepad_report_data[1] & 0x0000F000) >> (3*4 + 3) ) & 1;
        
        gamepad_data.tb1 = ((gamepad_report_data[1] & 0x000F0000) >> (3*4 + 4) ) & 1;
        gamepad_data.tb2 = ((gamepad_report_data[1] & 0x000F0000) >> (3*4 + 5) ) & 1;
        gamepad_data.tb3 = ((gamepad_report_data[1] & 0x000F0000) >> (3*4 + 6) ) & 1;
        gamepad_data.tb4 = ((gamepad_report_data[1] & 0x000F0000) >> (3*4 + 7) ) & 1;

        gamepad_data.stop = ((gamepad_report_data[1] & 0x00F00000) >> (3*4 + 8) ) & 1;
        gamepad_data.play = ((gamepad_report_data[1] & 0x00F00000) >> (3*4 + 9) ) & 1;

        return USBH_OK;
    }
  return USBH_FAIL;
}

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


/**
  * @}
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
