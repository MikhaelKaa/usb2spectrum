/**
 * @file cpld_spi.c
 * @author Михаил Каа
 * @brief 
 * 
 * @version 0.1
 * @date 2021-10-05
 * Date: 05.10.2021
 * @copyright Copyright (c) 2021
 */

#include <stdio.h>
#include <string.h>

#include "_spectrum.h"
#include "_spi.h"
volatile uint8_t config_8bit = 0b00000000;

void cpld_config(cpld_config_op_t op, cpld_config_num_t num) {
  if(num > 7) return;
  switch (op) {
  case cpld_config_bit_reset:
    config_8bit &= ~(1 << num);
    break;
  case cpld_config_bit_set:
    config_8bit |= (1 << num);
    break;
  case cpld_config_bit_toggle:
    config_8bit ^= (1 << num);
    break;
  
  default:
    return;
    break;
  }
  printf("config_8bit %x\r\n", config_8bit);
  send_data2cpld(&config_8bit, sizeof(config_8bit), spi_config);
}


void send_data2cpld(uint8_t* buf, uint8_t len, spi_adress_t adr) {
  switch (adr)
  {
  case spi_config:
  HAL_GPIO_WritePin(SPI_A0_GPIO_Port, SPI_A0_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(SPI_A1_GPIO_Port, SPI_A1_Pin, GPIO_PIN_RESET);
    break;

  case spi_mouse:
  HAL_GPIO_WritePin(SPI_A0_GPIO_Port, SPI_A0_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(SPI_A1_GPIO_Port, SPI_A1_Pin, GPIO_PIN_RESET);
    break;

  case spi_kmpstn:
  HAL_GPIO_WritePin(SPI_A0_GPIO_Port, SPI_A0_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(SPI_A1_GPIO_Port, SPI_A1_Pin, GPIO_PIN_SET);
    break;
    
  case spi_kbd:
  HAL_GPIO_WritePin(SPI_A0_GPIO_Port, SPI_A0_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(SPI_A1_GPIO_Port, SPI_A1_Pin, GPIO_PIN_SET);
    break;
  
  default:
  return;
    break;
  }

  CPLD_TRANSMIT(CPLD_SPI_PORT, buf, len);

}
