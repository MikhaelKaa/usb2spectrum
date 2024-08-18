/**
 * @file speccy_kb.c
 * @author Михаил Каа
 * @brief 
 * Эмулятор клавиатуры спектрум совместимых машин.
 * Матрица клавиатуры реализована в CPLD EPM7064.
 * Выгрузка данных состояния кнопок через spi.
 * Размер пакета 40 бит (5 байт).
 * @version 0.1
 * @date 2021-03-26
 * Date: 08.03.2021
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "_spectrum.h"
#include "EPM7064_spi.h"
#include "EPM7064_5x8.h"
#include "usbh_hid_keybd.h"

/**
 * @brief 
 * Массив для отправки в EPM.
 */
static uint8_t epm_buf[EPM_5X8_SIZE] = { 0x00, 0x00, 0x00, 0x00, 0x00 };
bool flag_shift  = false;
bool flag_ctrl  = false;
bool flag_alt   = false;
bool flag_del   = false;


void epm_5x8_remove(uint8_t code)
{

  epm_buf[0] &= ~table_5x8[code][0];
  epm_buf[1] &= ~table_5x8[code][1];
  epm_buf[2] &= ~table_5x8[code][2];
  epm_buf[3] &= ~table_5x8[code][3];
  epm_buf[4] &= ~table_5x8[code][4];

  switch (code)
  {
  case KEY_LEFTCONTROL:
    flag_ctrl = false;
    break;
  case KEY_LEFTALT:
    flag_alt = false;
    break;
  case KEY_DELETE:
    flag_del = false;
    break;

  default:
    break;
  }

  SP_DBG("USB key: %d\r\n", code);
  SP_DBG("5x8 KD4: %d\r\n", table_5x8[code][0]);
  SP_DBG("5x8 KD3: %d\r\n", table_5x8[code][1]);
  SP_DBG("5x8 KD2: %d\r\n", table_5x8[code][2]);
  SP_DBG("5x8 KD1: %d\r\n", table_5x8[code][3]);
  SP_DBG("5x8 KD0: %d\r\n", table_5x8[code][4]);
}

/**
 * @brief 
 * Отправка нулей при первом запуске. 
 */
void epm_init(void)
{
  memset(epm_buf, 0x00, EPM_5X8_SIZE);
  epm_5x8_flush();
}


/**
 * @brief 
 * Добавление кнопки по скан коду.
 * @param code - код кнопки от USB клавиатуры.
 */
void epm_5x8_add(uint8_t code)
{
  epm_buf[0] |= table_5x8[code][0];
  epm_buf[1] |= table_5x8[code][1];
  epm_buf[2] |= table_5x8[code][2];
  epm_buf[3] |= table_5x8[code][3];
  epm_buf[4] |= table_5x8[code][4];

switch (code)
  {
  // case KEY_F5:
  //   cpld_config(cpld_config_bit_toggle, cpld_config_mouse);
  //   break;
  // case KEY_F6:
  //   cpld_config(cpld_config_bit_toggle, cpld_config_kbd); 
  //   break;
  // case KEY_F7:
  //   cpld_config(cpld_config_bit_toggle, cpld_config_kmpstn); 
  //   break;
  // case KEY_F8:
  //   cpld_config(cpld_config_bit_toggle, cpld_config_fdd_swap_0); 
  //   break;
  // case KEY_F9:
  //   cpld_config(cpld_config_bit_toggle, cpld_config_128k_lock); 
  //   break;
  // case KEY_F10:
  //   cpld_config(cpld_config_bit_toggle, cpld_config_psg_A13); 
  //   break;
  // case KEY_F11:
  //   cpld_config(cpld_config_bit_toggle, cpld_config_out_1); 
  //   break;
  // case KEY_F4://KEY_PAUSE:
  //   cpld_config(cpld_config_bit_toggle, cpld_config_wait);
  //   SP_DBG("cpld toggle config_wait\n");
  //   break;

  case KEY_F12:
    opt_nmi(); 
    break;
    
  // case 224:
  //   flag_shift = true;
  //   SP_DBG("left shift - 224\r\n");
  //   break;

  case KEY_LEFTSHIFT:
    flag_ctrl = true;
    SP_DBG("flag_ctrl = true\r\n");
    break;
  case KEY_LEFTALT:
    flag_alt = true;
    SP_DBG("flag_alt = true\r\n");
    break;
  case KEY_DELETE:
    flag_del = true;
    SP_DBG("flag_del = true\r\n");
    if(flag_alt & flag_ctrl & flag_del) {
      opt_reset();
      flag_ctrl = false;
      flag_alt = false;
      flag_del = false;
    }
    break;

  default:
    break;
  }

  SP_DBG("USB key: %d\r\n", code);
  SP_DBG("5x8 KD4: %d\r\n", table_5x8[code][0]);
  SP_DBG("5x8 KD3: %d\r\n", table_5x8[code][1]);
  SP_DBG("5x8 KD2: %d\r\n", table_5x8[code][2]);
  SP_DBG("5x8 KD1: %d\r\n", table_5x8[code][3]);
  SP_DBG("5x8 KD0: %d\r\n", table_5x8[code][4]);
}


/**
 * @brief 
 * Загрузка массива в EPM7064.
 */
void epm_5x8_flush(void)
{
  // Отправка.
  EPM_TRANSMIT(EPM_SPI_PORT, epm_buf, EPM_5X8_SIZE);
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
  HAL_Delay(2);
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
}

void epm_5x8_flush_with_clear(void)
{
  // Отправка.
  EPM_TRANSMIT(EPM_SPI_PORT, epm_buf, EPM_5X8_SIZE);
  // Очистка массива кнопок.
  memset(epm_buf, 0x00, EPM_5X8_SIZE);
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
  HAL_Delay(2);
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
}