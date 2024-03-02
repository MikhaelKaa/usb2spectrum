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


void USBH_HID_EventCallback(USBH_HandleTypeDef *phost)
{
  // https://controllerstech.com/stm32-usb-host-hid/

  if(USBH_HID_GetDeviceType(phost) == HID_GAMEPAD)
  {
    // HID_GAMEPAD_Info_TypeDef *gamepad = USBH_HID_GetGamepadInfo(phost);
    // if (gamepad != 0) printf("GamePad: %08X %08X\r\n", gamepad[0], gamepad[1]);
    static GAMEPAD_Keys_TypeDef gp_old;
    GAMEPAD_Keys_TypeDef *gp = USBH_HID_GetGamepadInfo(phost);//(GAMEPAD_Keys_TypeDef*)gamepad;
    // printf("GamePad raw data: %08X %08X\r\n", gp[0], gp[1]);
    // printf("gamepad.axis_y %d\r\n", gp->axis_y);
    // printf("gamepad.axis_x %d\r\n", gp->axis_x);
    // printf("gamepad.left_1 %d\r\n", gp->left_1);
    // printf("gamepad.right_1 %d\r\n", gp->right_1);
    // printf("gamepad.left_2 %d\r\n", gp->left_2);
    // printf("gamepad.right_2 %d\r\n", gp->right_2);

    // printf("gamepad.button_1 %d\r\n", gp->button_1);
    // printf("gamepad.button_2 %d\r\n", gp->button_2);
    // printf("gamepad.button_3 %d\r\n", gp->button_3);
    // printf("gamepad.button_4 %d\r\n", gp->button_4);

    const uint8_t axis_gap = 10;
    if(memcmp(&gp_old, gp, sizeof(gp_old))) {
      if(gp->left_2 == 1) {
        if(gp->axis_x < 127) epm_5x8_add(KEY_5_PERCENT);
        if(gp->axis_x > 127) epm_5x8_add(KEY_8_ASTERISK);
        if(gp->axis_y < 127) epm_5x8_add(KEY_7_AMPERSAND);
        if(gp->axis_y > 127) epm_5x8_add(KEY_6_CARET);
        epm_5x8_add(KEY_LEFTCONTROL);
      } else {
        if(gp->axis_x < 127) epm_5x8_add(KEY_6_CARET);
        if(gp->axis_x > 127) epm_5x8_add(KEY_7_AMPERSAND);
        if(gp->axis_y < 127) epm_5x8_add(KEY_9_OPARENTHESIS);
        if(gp->axis_y > 127) epm_5x8_add(KEY_8_ASTERISK);
      }

      if(gp->left_1 == 1) {
        epm_5x8_add(KEY_LEFTCONTROL);
        epm_5x8_add(KEY_1_EXCLAMATION_MARK);
      }
      if(gp->right_2 == 1) epm_5x8_add(KEY_ENTER);
      if(gp->right_1 == 1) epm_5x8_add(KEY_R);

      if(gp->button_2 == 1) epm_5x8_add(KEY_0_CPARENTHESIS);
      if(gp->button_1 == 1) epm_5x8_add(KEY_9_OPARENTHESIS);

      if(gp->button_play == 1) epm_5x8_add(KEY_P);
      if(gp->button_stop == 1) {
        epm_5x8_add(KEY_LEFTCONTROL);
        epm_5x8_add(KEY_SPACEBAR);
      }
      epm_5x8_flush();
      if(gp->left_1 == 1 && gp->left_2 == 1 && gp->right_1 == 1 && gp->right_2 == 1) {
        opt_reset();
      }
    }
    memcpy(&gp_old, gp, sizeof(gp_old));
  }

  // Если устройство мышь
  if(USBH_HID_GetDeviceType(phost) == HID_MOUSE)
  {
    // �?нкрементируем счетчик событий мышки
    mouse_event_cnt++;
    role = use_mouse;
    HID_MOUSE_Info_TypeDef *Mouse_Info;
    // Заполняем поля структуры с данными мышки
    Mouse_Info = USBH_HID_GetMouseInfo(phost);
    // приводим к знаковому типу
    int X_Val = Mouse_Info->x;
    int Y_Val = Mouse_Info->y;
    if (X_Val > 127) X_Val -= 255;
    if (Y_Val > 127) Y_Val -= 255;

    // Отладочный вывод
    printf ("event %d: X=%d, Y=%d, Button1=%d, Button2=%d, Button3=%d\r\n",mouse_event_cnt, X_Val, Y_Val, \
            Mouse_Info->buttons[0],Mouse_Info->buttons[1], Mouse_Info->buttons[2]);

      if(Y_Val > gap) {HAL_GPIO_WritePin(DV1_GPIO_Port, DV1_Pin, GPIO_PIN_SET);}
      else {HAL_GPIO_WritePin(DV1_GPIO_Port, DV1_Pin, GPIO_PIN_RESET);}
      if(Y_Val < -gap) HAL_GPIO_WritePin(DV0_GPIO_Port, DV0_Pin, GPIO_PIN_SET);
      else {HAL_GPIO_WritePin(DV0_GPIO_Port, DV0_Pin, GPIO_PIN_RESET);}
      
      if(X_Val > gap) HAL_GPIO_WritePin(DV3_GPIO_Port, DV3_Pin, GPIO_PIN_SET);
      else {HAL_GPIO_WritePin(DV3_GPIO_Port, DV3_Pin, GPIO_PIN_RESET);}
      if(X_Val < -gap) {HAL_GPIO_WritePin(DV2_GPIO_Port, DV2_Pin, GPIO_PIN_SET);}
      else {HAL_GPIO_WritePin(DV2_GPIO_Port, DV2_Pin, GPIO_PIN_RESET);}


    if(Mouse_Info->buttons[0]) HAL_GPIO_WritePin(DV4_GPIO_Port, DV4_Pin, GPIO_PIN_SET);
    else {HAL_GPIO_WritePin(DV4_GPIO_Port, DV4_Pin, GPIO_PIN_RESET);}
  }

  // Если устройство клавиатура 
  if(USBH_HID_GetDeviceType(phost) == HID_KEYBOARD)
  {
      USBH_UsrLog("Key 2\r\n");
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
    printf ("Keys array: = %d : %d : %d : %d : %d : %d\r\n", \
    Keyboard_Info->keys[0], Keyboard_Info->keys[1], Keyboard_Info->keys[2], \
    Keyboard_Info->keys[3], Keyboard_Info->keys[4], Keyboard_Info->keys[5]);
    #endif /* KEY_DBG */

    // Проверяем кнопки shift и ctrl.
    if(Keyboard_Info->lshift || Keyboard_Info->rshift)  epm_5x8_add(KEY_LEFTCONTROL);
    if(Keyboard_Info->lctrl  || Keyboard_Info->rctrl )  epm_5x8_add(KEY_LEFTSHIFT);

    // Добавляем в матрицу нажатые кнопки.
    if(KEYS(0)) epm_5x8_add(KEYS(0));
    if(KEYS(1)) epm_5x8_add(KEYS(1));
    if(KEYS(2)) epm_5x8_add(KEYS(2));
    if(KEYS(3)) epm_5x8_add(KEYS(3));
    if(KEYS(4)) epm_5x8_add(KEYS(4));
    if(KEYS(5)) epm_5x8_add(KEYS(5));
    
    epm_5x8_flush();

    if(CHECK_KEYS_ARRAY(69)) opt_nmi();
    if(CHECK_KEYS_ARRAY(76) && \
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
