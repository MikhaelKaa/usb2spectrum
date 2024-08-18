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
char gp_logo0[] = "        . . . . .                     . . . . . \r\n\
      .  _______  .                 .  _______  .\r\n\
      . |left_2 | .                 . |right_2| .\r\n\
      .  _______    . . . . . . . .    _______  .\r\n\
      . |left_1 |      Defender       |right_1| .\r\n\
    .                                              .\r\n\
   .    _             clear                (1)       .\r\n\
  .   _| |_     turbo   O    auto                      .\r\n\
  .  |_   _|      O           O      (4)         (2)   .\r\n\
   .   |_|                                            .\r\n\
    .               =       >              (3)       .\r\n\
      .           stop(9)   play(10)              .\r\n\
          .    .    .   .    .     .    .     .\r\n\
                      with autofire\r\n";



// char gp_logo1[] = "                                                        .OW0o'\r\n\     
//                                                         'kXkc.                                                          \r\n\
//                                                        .ck0xl'                                                          \r\n\
//                                                        :O000Ol.                                                         \r\n\     
//                                                       .lO0000d.                                                         \r\n\
//                                                       .lO0000x.                                                         \r\n\
//                                                       .oO0000x'                        ..',,,,,''...                    \r\n\
//                  .,;:::cccc::;,.                     .,dO0000k;..                   .,lxxkkkkkkkkxxd:..                 \r\n\
//               .'cxkkkkkkkkkkkkko'              ..,:lodxxkkkkkxxdooc;...            'okkkkkkkkkkkOOkkkko'                \r\n\
//              .lxOOOOOOkOOkkOkkkOkl.         .,coxkkOOOOOOkkkOOkkkkkkkdo:.........':dkkkkkkkkkkkkkkkkkkkl.               \r\n\
//             .:dOOOOOOkkOkkkkkkkkkkdllllllc;:oxkkOOOOkkkkkkkkkkkkkkkkkkkkxoldxxxkkkkkkkkkkkkkkkkkkkkkkxkd'               \r\n\
//             .lxkkkkkkkkkkkkkkkkkkkxxkkkkxoodkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkxkkkkkkkkkkkkkkkkkkkkkkkkkkkxl.              \r\n\
//            .;oxxkkOOOOOkkkkkkkkkkkkxxxxxxxkkOOkkkkkkkkkkkkkkkkkxxkkkkkkkOOkOkkkkxxxkkkOOkkkkOOOOOkkkkkOkkd;.            \r\n\
//          .;lxkOOOOOOOOOOOkkkkkkkkkkkxxkkkkkO00Oxc;;co::c::cccclc;:coxkO000OkkkkxxxkOkkkkkOOOOOOOOOOkkkkkkkkkl.          \r\n\
//        .:dkOOOOO0KK0000KKKKK00kkkkkkkOOkkkkOOOkxc;;cc:;;::;:c::c:;:lxkkOOkkkkdoxkkkkkkkkO0OOOkdkOO0Okkkkkkkkkx:.        \r\n\
//       ,lxOOOO00OxxdxO00OOO00KKK0OkkkkkOOkkkkkkkkxxxkkkkkkkkkkkkkkkkkkkkkkkkxookOkkkkkkkkO00O0OxkOO0Okkkkkkkkkkkl.       \r\n\
//      ,okOOO0KOoclokKNNNNX0kkOO00K0kxxxkkOkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkxookkkkkkkkkkkk0XXKOkxk0OkkkkkkkkkkkkOo.      \r\n\
//     ,okOOO0KxodxxOXNNNNNNX0kkOOO000kkkkkkOkkkkkkkkOOOkkkkkkkkkkOOOOOOOOOOkdoxOkkkOOOOOkkkkO0K0OkOOkkkkOOOOOOOkOOOc      \r\n\
//    .lxOOO0KxokOOKXXNXXNNXXK0kkkO0000kkkxkOOOOOOOOOOOOxloolllccokOOOOOOOOOkodkkOOOOOkOO0OOkkkkkkkkkkkO00kddxOOOOkOx,     \r\n\
//   .:dkOOOXOlxOO0KKXXXNNX0Okxdddk0KK0OkkkkkO0Odoollllodkko;:dOkOkoloooodO0koxkO00OkxdxkO00OOOOOOOOOOO00OkoclkO0OkkOl.    \r\n\
//   .cxOOO0XkoOOkkOOOOO0kooooooodk0XXK0OOOkkO00kxdlcodxk00o.'d000Oxdl:ldk00OxkOOKK0Okxxkk00OOOOOOOOOOOKK0OocdkO00kOOo.    \r\n\
//   'lkOOO0X0OXKOkxxkkkkdooooodOKOOXNK0OOOOk00000x,.l000000kk00000KO:.,kK000OkOO0XXK0OkkO0OOOOOOOOOOOO0XXKOxodO0OOOkl.    \r\n\
//   'lkOOOOKKKNNX0OkkkkkkkxdxO0KKO0NNKOOOOOO0K000Oocx0K000000000KKK0dlx0K00K0OOOOOKKKK0OOOOOOOOO000OOOOO0K0OkOOOOOOkl,.   \r\n\
//  .;okOO0O0KKXNNXOxkkkkkkkk0Kkdx0NWX000OOO0KKKKKKkdkKKK0K0000000K0kdokKK00KXKOOOOOOOOOOOOOO00OkkkO00OOOOOOOOOOOOOOOd:.   \r\n\
//  .lxkO0OOO0KKXNX0kxkkOkkkO0OooONWX00000OO0KXXKxc:;;:d0K0000000KO:',;:cd0KKXXK0OOOOOOOOOOO00OOkxdxO000OOOOOOOOOOOOOxc.   \r\n\
//  'okkO00O0O00KXXXXK00K0OkkO0XNWNK00000OOO0XXXXd.....c0KKKKKKKKKk;.',;cd0KKXNNXK0OOOOOOO0OKX00OkxxkO00OOOOOOOOOOOkOOo'   \r\n\
//  ,okOkOK00O0O000XNNNNNNNNNNNNXK00000kxxk0XXXXXKkddddxO0000000000kkk0KKKKKXXXNNXK0000000000KXX0kxdk00OOOOOOOkkkkkkk0x;.  \r\n\
// .:dkOO0KKK0000OOO00KXXXXXXKK000000Oxxk0KXKOkkkkkkkkkkOOOOOOOOOO0000KKKKKKXKKKXNXXKK000000000KK0000OOOOOOkddxkkkkkk0Oc.  \r\n\
// .:dxOOKNKKKKKK0OOO00000000000000OkxkOkxkkxxxxkkkkkkxxxddddooooodddxxxkkO00KKKXXNNXKKKKK0000OOOOOOOOkxdddodxkkkkkkk00d'  \r\n\
// .cokOOKNNNXXKKKKKKK00000KK0OOOOkkOOkxddkkkkxdol:;,,'...............'',;:clodxkO0KXXK00000000OkkkkkkddddkkOOOOOkkkkOKk:. \r\n\
// .cdkOOKNNNWWNXXXXKKKKKKKK0OkkkkkkO0Okkxoc;'...                           ...';:ldxkO0OOOOO000OkOOOOOOOOOOOOOOOkkkkOKOo. \r\n\
// .cdk00KNNNNNNWWWWNNXXXXXK0OOOkkkkkkdc,'.                                       ..,lxkOOOOO0KK00000000000OOOOOOOkkkO00o' \r\n\
// 'ldk00XNNNNNNWWWWNNNNNNXK0OOkkkkkxo'                                              .,oxOOOOO00KKK0000000000OOOOOOOkO00d; \r\n\
// ,ldO00NNNNNNNWWWWWNNWNXK0OOkkkkxdc.                                                 .lxkOOOO00KKK00000000000000OOOO00xc.\r\n\
// ;oxO0KNNNNNNNWWWWNNNNXK0OOkkkkkdc.                                                   .cxOOOOO00KK00000000000000OOOkO0kl.\r\n\
// :oxOKXNNNNNNWWWNNNNNXK0Okkkkkkdc.                                                     .cxkOOOO00KK0000000000000OOOkkOko,\r\n\
// cdk0KNNNNNNNNWNNNNXKK0Okkkkkkdl'                                                       .cxkOOOOO00000000000000OOOOkkOkd:\r\n\
// cdk0XNNNNNNNNNNNNXK00Okkkkkkxl'                                                         .cxkOOOOO000000000000OOOOOkkkkxc\r\n\
// lxOKNNNNNNNNNNNXK000OkOkkkkxo,                                                           .:dkOOOOOO0000000000OOOOOOkkkkl\r\n\
// oxOXNNNNNNNNNXK000OOOOOOkkxo,                                                             .:dkOOOOOOO000000000OOOOOkkkko\r\n\
// okKNNNNNNNNXK000OOOOOOOOkkd,                                                               .:dkOOOOOOOOO0000000OOOOOkkko\r\n\
// lkKNNNNNNXK0000OOOOOOOOOkd;                                                                 .:dkOOOOOOOOO0000000OOOOOkdc\r\n\
// :oOKNNNNNK0000OOOOOOOOOkx:                                                                   .:dkOOOOOOOOOO000000OOOkxl,\r\n\
// .;ok0KXKK000OOOOOOOOOOkx:.                                                                     ;okkOOOOOOOOOOOOOOOOxdc' \r\n\
//   'cdxOOO0OOOOOOOOOOOkd,                                                                        .:oxkOOOOOOOOOOOkxdc,.  \r\n\
//    ..:odxkOOOOOOOOkdc,.                                                                           .':odxkkkkkxdoc,..    \r\n\
//       .,:oxxxxkxdl,.                                                                                 ..,;:cc:;'.        \r\n";





volatile int USBH_HID_GamepadDecode_flag = -1;
static GAMEPAD_Keys_TypeDef0 gp_last0;
static GAMEPAD_Keys_TypeDef0 gp_last1;

const uint64_t gp_defender_old_ptrn = 0x7f80800100000f7f;
const uint64_t gp_defender_new_ptrn = 0x80007f7f00000f80;
const uint8_t axis_gap = 10;
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

#define KEY_DBG (1U)

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

    #define KEMPSTON_UP     DV3_GPIO_Port, DV3_Pin
    #define KEMPSTON_DOWN   DV2_GPIO_Port, DV2_Pin
    #define KEMPSTON_LEFT   DV1_GPIO_Port, DV1_Pin
    #define KEMPSTON_RIGHT  DV0_GPIO_Port, DV0_Pin
    #define KEMPSTON_FIRE   DV4_GPIO_Port, DV4_Pin

void type_0(GAMEPAD_Keys_TypeDef0 *gp) {

    // Kempston
      if((gp->axis_y < 127) || (gp->button_1 == 1)) {HAL_GPIO_WritePin(KEMPSTON_UP, GPIO_PIN_SET);}
      else {HAL_GPIO_WritePin(KEMPSTON_UP, GPIO_PIN_RESET);}
      if((gp->axis_y > 127) ) {HAL_GPIO_WritePin(KEMPSTON_DOWN, GPIO_PIN_SET);}
      else {HAL_GPIO_WritePin(KEMPSTON_DOWN, GPIO_PIN_RESET);}
      if((gp->axis_x < 127) ) {HAL_GPIO_WritePin(KEMPSTON_LEFT, GPIO_PIN_SET);}
      else {HAL_GPIO_WritePin(KEMPSTON_LEFT, GPIO_PIN_RESET);}
      if((gp->axis_x > 127) ) {HAL_GPIO_WritePin(KEMPSTON_RIGHT, GPIO_PIN_SET);}
      else {HAL_GPIO_WritePin(KEMPSTON_RIGHT, GPIO_PIN_RESET);} 
      if(gp->button_2 == 1) {HAL_GPIO_WritePin(KEMPSTON_FIRE, GPIO_PIN_SET);}
      else {HAL_GPIO_WritePin(KEMPSTON_FIRE, GPIO_PIN_RESET);}

    // Keyboard
    if(memcmp(&gp_last0, gp, sizeof(gp_last0))) {

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
      if(gp->left_1 == 1 && gp->left_2 == 1) {
        opt_nmi();
      }
      if(gp->left_1 == 1 && gp->left_2 == 1 && gp->right_1 == 1 && gp->right_2 == 1) {
        opt_reset();
      }
      printf("GamePad t0 raw: 0x%08lx%08lx\r\n", gp[0], gp[1]);
    }
    memcpy(&gp_last0, gp, sizeof(gp_last0));
}

void type_1(GAMEPAD_Keys_TypeDef1 *gp) {
    // Kempston
      if((gp->axis_y < 127) || (gp->button_1 == 1)) {HAL_GPIO_WritePin(KEMPSTON_UP, GPIO_PIN_SET);}
      else {HAL_GPIO_WritePin(KEMPSTON_UP, GPIO_PIN_RESET);}
      if((gp->axis_y > 127) ) {HAL_GPIO_WritePin(KEMPSTON_DOWN, GPIO_PIN_SET);}
      else {HAL_GPIO_WritePin(KEMPSTON_DOWN, GPIO_PIN_RESET);}
      if((gp->axis_x < 127) ) {HAL_GPIO_WritePin(KEMPSTON_LEFT, GPIO_PIN_SET);}
      else {HAL_GPIO_WritePin(KEMPSTON_LEFT, GPIO_PIN_RESET);}
      if((gp->axis_x > 127) ) {HAL_GPIO_WritePin(KEMPSTON_RIGHT, GPIO_PIN_SET);}
      else {HAL_GPIO_WritePin(KEMPSTON_RIGHT, GPIO_PIN_RESET);} 
      if(gp->button_2 == 1) {HAL_GPIO_WritePin(KEMPSTON_FIRE, GPIO_PIN_SET);}
      else {HAL_GPIO_WritePin(KEMPSTON_FIRE, GPIO_PIN_RESET);}

    // Keyboard
    
    if(memcmp(&gp_last1, gp, sizeof(gp_last1))) {

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
      if(gp->left_1 == 1 && gp->left_2 == 1) {
        opt_nmi();
      }
      if(gp->left_1 == 1 && gp->left_2 == 1 && gp->right_1 == 1 && gp->right_2 == 1) {
        opt_reset();
      }
      printf("GamePad t1 raw: 0x%08lx%08lx\r\n", gp[0], gp[1]);
    }
    memcpy(&gp_last1, gp, sizeof(gp_last1));
}

void USBH_HID_EventCallback(USBH_HandleTypeDef *phost)
{
  // https://controllerstech.com/stm32-usb-host-hid/

  if(USBH_HID_GetDeviceType(phost) == HID_GAMEPAD)
  {
    role = use_gamepad;
    uint32_t *p = USBH_HID_GetGamepadInfo(phost);
    GAMEPAD_Keys_TypeDef0 *gp0 = p;
    GAMEPAD_Keys_TypeDef1 *gp1 = p;

    // detect
    if(-1 == USBH_HID_GamepadDecode_flag ) {
    printf("\r\n <------> Game pad detect start.\r\n");
    printf("GamePad raw data:  0x%08lx%08lx\r\n", p[0], p[1]);

    if(p[0] == 0x7F808001 && p[1] == 0x00000F7F){
      USBH_HID_GamepadDecode_flag = 0;
      printf("\r\n%s", gp_logo0);
      printf("hit %d\r\n", USBH_HID_GamepadDecode_flag);
      return;
    }

    if(p[0] == 0x80007F7F && p[1] == 0x00000F80) {
      USBH_HID_GamepadDecode_flag = 1;
      printf("\r\n%s", gp_logo0);
      printf("hit %d\r\n", USBH_HID_GamepadDecode_flag);
      return;
    }

    USBH_HID_GamepadDecode_flag = -2;
    printf("hit unknown %d\r\n", USBH_HID_GamepadDecode_flag);
    return;
    }


    // proc
    switch (USBH_HID_GamepadDecode_flag)
    {
    case -2: // unknown
      return;
      break;

    case -1: // no init
      return;
      break;

    case 0: // old
      type_0(gp0);
      break;

    case 1: // new
      type_1(gp1);
      break;
    
    default:
      break;
    }
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
    #if(KEY_DBG == 1U)
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

