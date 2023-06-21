/**
 * @file opt_buttons.c
 * @author Михаил Каа
 * @brief 
 * Дополнительные GPIO. Управление RESET и NMI/MAGIC.
 * Вывод RESET типа открытый коллектор,
 * транзисторный ключ с резистором ограничивающим ток разряда конденсатора 10 мкФ в цепи сброса Спектрума.
 * Управление NMI для Скорпиона осуществляется через GPIO типа открытый коллектор.
 * Управление NMI для Пентагона осуществляется через NMI_GATE в EPM7064.
 * @version 0.1
 * @date 2021-03-26
 * Date: 13.03.2021
 * @copyright Copyright (c) 2021
 * 
 */

//#include <stdio.h>
#include "_spectrum.h"

/**
 * @brief 
 * Счетчики измерения вмерени от Make до Break.
 */
static int32_t reset = 0, magic = 0;

/**
 * @brief 
 * Обновление управлением GPIO. Отпускает нажатую кнопку по таймауту.
 */
void opt_update(void)
{
  if(reset > 0) reset--;
  if(reset == 0) {
    reset--;
    HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_RESET);
    SP_DBG("Break button RESET\n");
  }

  if(magic > 0) magic--;
  if(magic == 0) {
    magic--;
    HAL_GPIO_WritePin(MAGIC_GPIO_Port, MAGIC_Pin, GPIO_PIN_SET);
    SP_DBG("Break button NMI\n");
  }
}

/**
 * @brief 
 * Сброс компьтера.
 */
void opt_reset(void)
{
  HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_SET);
  SP_DBG("Make button RESET: %d\n", OPT_RESET);
  reset = OPT_RESET;
}

/**
 * @brief 
 * Кнопка NMI (MAGIC).
 */
void opt_nmi(void)
{
  HAL_GPIO_WritePin(MAGIC_GPIO_Port, MAGIC_Pin, GPIO_PIN_RESET);
  SP_DBG("Make button NMI: %d\n", OPT_NMI);
  magic = OPT_NMI;
}
