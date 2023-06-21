/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file            : usb_host.c
  * @version         : v2.0_Cube
  * @brief           : This file implements the USB Host
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

/* Includes ------------------------------------------------------------------*/

#include "usb_host.h"
#include "usbh_core.h"
#include "usbh_hid.h"

/* USER CODE BEGIN Includes */
#include "spectrum.h"
#include "_spectrum.h"
/* USER CODE END Includes */

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
extern dev_role_t role;
/* USER CODE END PFP */

/* USB Host core handle declaration */
USBH_HandleTypeDef hUsbHostFS;
ApplicationTypeDef Appli_state = APPLICATION_IDLE;

/*
 * -- Insert your variables declaration here --
 */
/* USER CODE BEGIN 0 */

#define KEYS(x) (Keyboard_Info->keys[x])
#define CHECK_KEYS_ARRAY(x) (KEYS(0) == (x) || KEYS(1) == (x) ||  KEYS(2) == (x) ||  KEYS(3) == (x) ||  KEYS(4) == (x))

#define KEY_DBG

typedef enum kempston{
  Right = 0,
  Left,
  Down,
  Up,
  Fire
}kempston_t;
static int gap = 0;

void static inline set_kempston_by_kbd(GPIO_TypeDef* port, uint16_t pin, HID_KEYBD_Info_TypeDef *Keyboard_Info, uint8_t key_code) {
  if(CHECK_KEYS_ARRAY(key_code)) {
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
  } else {
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
  }
}

extern SPI_HandleTypeDef hspi2;
uint32_t gamepad_0, gamepad_1;
void USBH_HID_EventCallback(USBH_HandleTypeDef *phost)
{
  // https://controllerstech.com/stm32-usb-host-hid/

  if(USBH_HID_GetDeviceType(phost) == HID_GAMEPAD)
  {
    HID_GAMEPAD_Info_TypeDef *gamepad = USBH_HID_GetGamepadInfo(phost);
    if (gamepad == NULL) return; 

    if((gamepad->data_0 != gamepad_0) | (gamepad->data_1 != gamepad_1)) {

      static uint8_t kmpstn_data;
      kmpstn_data = gamepad->left << 1 | gamepad->right << 0 | gamepad->down << 2 |\
        gamepad->up << 3 | gamepad->b1 << 4 | gamepad->b2 << 5 | gamepad->b3 << 6 | gamepad->b4 << 7;
      send_data2cpld(&kmpstn_data, sizeof(kmpstn_data), spi_kmpstn);
      printf("raw data: %08X %08X\n", gamepad->data_0, gamepad->data_1);
      printf("up:%d down:%d left:%d right:%d b1:%d b2:%d b3:%d b4:%d \n\
        tb1:%d tb2:%d tb3:%d tb4:%d  stop:%d play:%d \n",\
       gamepad->up, gamepad->down, gamepad->left, gamepad->right,\
       gamepad->b1, gamepad->b2, gamepad->b3, gamepad->b4,\
       gamepad->tb1, gamepad->tb2, gamepad->tb3, gamepad->tb4, gamepad->stop, gamepad->play);
      printf("kempstn: %02X\n", kmpstn_data);
      
    }

    gamepad_0 = gamepad->data_0;
    gamepad_1 = gamepad->data_1;

  }

  // Если устройство мышь
  if(USBH_HID_GetDeviceType(phost) == HID_MOUSE)
  {
    if(role != use_mouse) cpld_config(cpld_config_bit_set, cpld_config_mouse);
    // �?нкрементируем счетчик событий мышки
    mouse_event_cnt++;
    role = use_mouse;
    HID_MOUSE_Info_TypeDef *Mouse_Info;
    // Заполняем поля структуры с данными мышки
    Mouse_Info = USBH_HID_GetMouseInfo(phost);

    const float m_scale = 0.42;
    static float x, y;
    x += (float)*(int8_t*)(&Mouse_Info->x) * m_scale;
    y -= (float)*(int8_t*)(&Mouse_Info->y) * m_scale;

    if(x < 0) x = 255;
    if(x > 255) x = 0;
    if(y < 0) y = 255;
    if(y > 255) y = 0;

    
    static uint8_t whell;
    if(Mouse_Info->whell == 1)   {whell++; /*cpld_config(cpld_config_bit_set, cpld_config_kbd); return;*/ }
    if(Mouse_Info->whell == 255) {whell--; /*cpld_config(cpld_config_bit_reset, cpld_config_kbd); return;*/ }
    

    static uint8_t mouse_data[3];
    mouse_data[0] = y;
    mouse_data[1] = x;
    mouse_data[2] = (whell<<4) | 0b00001000 | (!Mouse_Info->buttons[0]) << 0 | (!Mouse_Info->buttons[1]) << 1 | (!Mouse_Info->buttons[2]) << 2;
    send_data2cpld(mouse_data, sizeof(mouse_data), spi_mouse);

    /*HAL_GPIO_WritePin(SPI_A0_GPIO_Port, SPI_A0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(SPI_A1_GPIO_Port, SPI_A1_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi2, mouse_data, sizeof(mouse_data), 10);*/
    
    //printf ("event %d: X=%d\r\n",mouse_event_cnt, Mouse_Info->whell);

  }

  // Если устройство клавиатура 
  if(USBH_HID_GetDeviceType(phost) == HID_KEYBOARD)
  {
      USBH_UsrLog("Key 2\n");
    role = use_kbd;
    //uint8_t ext_key[2] = {0, 0};
    HID_KEYBD_Info_TypeDef *Keyboard_Info;

    // Заполняем поля структуры с данными клавиатуры
    Keyboard_Info = USBH_HID_GetKeybdInfo(phost);

    // Получаем ASCII код последней нажатой клавиши
    //key = USBH_HID_GetASCIICode(Keyboard_Info);
    //printf ("Key Pressed = %d \t %c\n", key, key);

    // Отладочный вывод
    #ifdef KEY_DBG
    printf ("Keys array: = %d : %d : %d : %d : %d : %d\n", \
    Keyboard_Info->keys[0], Keyboard_Info->keys[1], Keyboard_Info->keys[2], \
    Keyboard_Info->keys[3], Keyboard_Info->keys[4], Keyboard_Info->keys[5]);
    #endif /* KEY_DBG */

    // Проверяем кнопки shift и ctrl.
    if(Keyboard_Info->lshift || Keyboard_Info->rshift)  epm_5x8_add(KEY_LEFTCONTROL);
    if(Keyboard_Info->lctrl  || Keyboard_Info->rctrl )  epm_5x8_add(KEY_LEFTSHIFT);
    // alt for reset.
    if(Keyboard_Info->lalt  || Keyboard_Info->ralt )    epm_5x8_add(KEY_LEFTALT);
    

    // Добавляем в матрицу нажатые кнопки.
    if(KEYS(0)) epm_5x8_add(KEYS(0));
    if(KEYS(1)) epm_5x8_add(KEYS(1));
    if(KEYS(2)) epm_5x8_add(KEYS(2));
    if(KEYS(3)) epm_5x8_add(KEYS(3));
    if(KEYS(4)) epm_5x8_add(KEYS(4));
    if(KEYS(5)) epm_5x8_add(KEYS(5));
    
    epm_5x8_flush_usb();


    //if(CHECK_KEYS_ARRAY(69)) opt_nmi();
    //if(CHECK_KEYS_ARRAY(76) && \
    (Keyboard_Info->lctrl || Keyboard_Info->rctrl) && \
    (Keyboard_Info->lctrl || Keyboard_Info->rctrl) ) opt_reset();
    
    return;

    // Нажимает на порты кемпстона если нажата соответствующая кнопка
    set_kempston_by_kbd(DV0_GPIO_Port, DV0_Pin, Keyboard_Info, 26);
    set_kempston_by_kbd(DV1_GPIO_Port, DV1_Pin, Keyboard_Info, 22);
    set_kempston_by_kbd(DV0_GPIO_Port, DV0_Pin, Keyboard_Info, 82);
    set_kempston_by_kbd(DV1_GPIO_Port, DV1_Pin, Keyboard_Info, 81);
    
    set_kempston_by_kbd(DV2_GPIO_Port, DV2_Pin, Keyboard_Info, 4);
    set_kempston_by_kbd(DV3_GPIO_Port, DV3_Pin, Keyboard_Info, 7);
    set_kempston_by_kbd(DV2_GPIO_Port, DV2_Pin, Keyboard_Info, 80);
    set_kempston_by_kbd(DV3_GPIO_Port, DV3_Pin, Keyboard_Info, 79);

    set_kempston_by_kbd(DV4_GPIO_Port, DV4_Pin, Keyboard_Info, 25);
    set_kempston_by_kbd(DV4_GPIO_Port, DV4_Pin, Keyboard_Info, 44);
  }
}
/* USER CODE END 0 */

/*
 * user callback declaration
 */
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id);

/*
 * -- Insert your external function declaration here --
 */
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
  * Init USB host library, add supported class and start the library
  * @retval None
  */
void MX_USB_HOST_Init(void)
{
  /* USER CODE BEGIN USB_HOST_Init_PreTreatment */

  /* USER CODE END USB_HOST_Init_PreTreatment */

  /* Init host Library, add supported class and start the library. */
  if (USBH_Init(&hUsbHostFS, USBH_UserProcess, HOST_FS) != USBH_OK)
  {
    Error_Handler();
  }
  if (USBH_RegisterClass(&hUsbHostFS, USBH_HID_CLASS) != USBH_OK)
  {
    Error_Handler();
  }
  if (USBH_Start(&hUsbHostFS) != USBH_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_HOST_Init_PostTreatment */

  /* USER CODE END USB_HOST_Init_PostTreatment */
}

/*
 * Background task
 */
void MX_USB_HOST_Process(void)
{
  /* USB Host Background task */
  USBH_Process(&hUsbHostFS);
}
/*
 * user callback definition
 */
static void USBH_UserProcess  (USBH_HandleTypeDef *phost, uint8_t id)
{
  /* USER CODE BEGIN CALL_BACK_1 */
  switch(id)
  {
  case HOST_USER_SELECT_CONFIGURATION:
  break;

  case HOST_USER_DISCONNECTION:
  Appli_state = APPLICATION_DISCONNECT;
  break;

  case HOST_USER_CLASS_ACTIVE:
  Appli_state = APPLICATION_READY;
  break;

  case HOST_USER_CONNECTION:
  Appli_state = APPLICATION_START;
  break;

  default:
  break;
  }
  /* USER CODE END CALL_BACK_1 */
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
