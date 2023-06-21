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

// Адресация регистров внутри CPLD.
typedef enum spi_adress {
    spi_config = 0,
    spi_mouse,
    spi_kmpstn,
    spi_kbd,
}spi_adress_t;

// Отправка данных в регистры CPLD
void send_data2cpld(uint8_t* buf, uint8_t len, spi_adress_t adr);

uint16_t ps2_raw_data;

// 
typedef enum cpld_config_op {
    cpld_config_bit_reset = 0,
    cpld_config_bit_set,
    cpld_config_bit_toggle
}cpld_config_op_t;

typedef enum cpld_config_num {
    cpld_config_mouse = 0,
    cpld_config_kbd,
    cpld_config_kmpstn,
    cpld_config_fdd_swap_0,
    cpld_config_128k_lock,
    cpld_config_psg_A13,
    cpld_config_out_1,
    cpld_config_wait
}cpld_config_num_t;

void cpld_config(cpld_config_op_t op, cpld_config_num_t num);
/*void cpld_toogle_config_8bit(uint8_t num);
void cpld_set_config_8bit(uint8_t num);
void cpld_reset_config_8bit(uint8_t num);*/

/**
 * @brief 
 * Отправка нулей при первом запуске. Если этого не делать и нет аппаратной подтяжки выводов, то CPLD антенит помехи и генерит.
 */
void epm_init(void);

/**
 * @brief 
 * Добавление кнопки по скан коду.
 * @param code 
 */
void epm_5x8_add(uint8_t code);

void epm_5x8_remove(uint8_t code);

void epm_5x8_flush_ps2(void);

/**
 * @brief 
 * Отправка подготовленного массива кнопок.
 */
void epm_5x8_flush_usb(void);

/**
 * @brief 
 * Обновление счетчиками GPIO.
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