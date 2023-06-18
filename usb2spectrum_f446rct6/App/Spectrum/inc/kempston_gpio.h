#ifndef SPECTRUM_INTERNAL
#define SPECTRUM_INTERNAL

/**
 * @file kempston_gpio.h
 * @author Михаил Каа
 * @brief Стандартный пятикнопочный кемпстон порт.
 * @version 0.1
 * @date 2021-03-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "_spectrum.h"

#define KEMP_MAKE     GPIO_PIN_RESET
#define KEMP_BRAKE    GPIO_PIN_SET

#define KEMP_MAKE_UP      HAL_GPIO_WritePin(DV4_GPIO_Port, DV1_Pin, KEMP_MAKE);
#define KEMP_MAKE_DOWN    HAL_GPIO_WritePin(DV3_GPIO_Port, DV1_Pin, KEMP_MAKE);
#define KEMP_MAKE_LEF     HAL_GPIO_WritePin(DV2_GPIO_Port, DV1_Pin, KEMP_MAKE);
#define KEMP_MAKE_RIGHT   HAL_GPIO_WritePin(DV1_GPIO_Port, DV1_Pin, KEMP_MAKE);
#define KEMP_MAKE_FIRE    HAL_GPIO_WritePin(DV0_GPIO_Port, DV1_Pin, KEMP_MAKE);

#define KEMP_BRAKE_UP     HAL_GPIO_WritePin(DV4_GPIO_Port, DV1_Pin, KEMP_BRAKE);
#define KEMP_BRAKE_DOWN   HAL_GPIO_WritePin(DV3_GPIO_Port, DV1_Pin, KEMP_BRAKE);
#define KEMP_BRAKE_LEF    HAL_GPIO_WritePin(DV2_GPIO_Port, DV1_Pin, KEMP_BRAKE);
#define KEMP_BRAKE_RIGHT  HAL_GPIO_WritePin(DV1_GPIO_Port, DV1_Pin, KEMP_BRAKE);
#define KEMP_BRAKE_FIRE   HAL_GPIO_WritePin(DV0_GPIO_Port, DV1_Pin, KEMP_BRAKE);

#endif /* SPECTRUM_INTERNAL */
