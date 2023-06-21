#ifndef SPECTRUM_INTERNAL
#define SPECTRUM_INTERNAL

/**
 * @file _spi.h
 * @author Михаил Каа
 * @brief 
 * Отправка данных в CPLD через SPI.
 * @version 0.1
 * @date 2021-03-26
 * Date: 08.03.2021
 * @copyright Copyright (c) 2021
 * 
 */

#include "_spectrum.h"

/**
 * @brief 
 * Отправка в блокирующем режиме.
 * 40 MBit, аппаратный NSS, 8 bit.
 */
extern  SPI_HandleTypeDef               hspi2;
#define CPLD_SPI_PORT                    &hspi2
#define CPLD_TRANSMIT(port, buf, size)   HAL_SPI_Transmit((port), (buf), (size), 10)

#endif /* SPECTRUM_INTERNAL */
