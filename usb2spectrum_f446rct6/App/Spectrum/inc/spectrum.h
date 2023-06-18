#ifndef SPECTRUM
#define SPECTRUM

/**
 * @file spectrum.h
 * @author Михаил Каа
 * @brief Эмулятор периферии ввода спектрум совместимых машин.
 * @version 0.1
 * @date 2021-03-26
 * Date: 08.03.2021
 * @copyright Copyright (c) 2021
 * 
 */

#include "main.h"

/**
 * @brief 
 * Добавление кнопки по скан коду.
 * @param code 
 */
void epm_5x8_add(uint8_t code);

/**
 * @brief 
 * Отправка подготовленного массива кнопок.
 */
void epm_5x8_flush(void);

/**
 * @brief 
 * Обновление счетчиками GPIO.н
 */
void opt_update(void);

/**
 * @brief 
 * Кнопка RESET.
 * 
 */
void opt_reset(void);

/**
 * @brief 
 * Кнопка NMI (MAGIC).
 */
void opt_nmi(void);

#endif /* SPECTRUM */