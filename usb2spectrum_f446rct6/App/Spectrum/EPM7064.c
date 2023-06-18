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

#include "_spectrum.h"
#include "EPM7064_spi.h"
#include "EPM7064_5x8.h"

/**
 * @brief 
 * Массив для отправки в EPM.
 */
static uint8_t epm_buf[EPM_5X8_SIZE] = { 0x00, 0x00, 0x00, 0x00, 0x00 };

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

  SP_DBG("USB key: %d\n", code);
  SP_DBG("5x8 KD4: %d\n", table_5x8[code][0]);
  SP_DBG("5x8 KD3: %d\n", table_5x8[code][1]);
  SP_DBG("5x8 KD2: %d\n", table_5x8[code][2]);
  SP_DBG("5x8 KD1: %d\n", table_5x8[code][3]);
  SP_DBG("5x8 KD0: %d\n", table_5x8[code][4]);
}

/**
 * @brief 
 * Загрузка массива в EPM7064.
 */
void epm_5x8_flush(void)
{
  // Отправка.
  HAL_GPIO_WritePin(SPI2_NSS_GPIO_Port, SPI2_NSS_Pin, GPIO_PIN_RESET);
  EPM_TRANSMIT(EPM_SPI_PORT, epm_buf, EPM_5X8_SIZE);
  HAL_GPIO_WritePin(SPI2_NSS_GPIO_Port, SPI2_NSS_Pin, GPIO_PIN_SET);
  // Очистка массива кнопок.
  memset(epm_buf, 0x00, EPM_5X8_SIZE);
}
