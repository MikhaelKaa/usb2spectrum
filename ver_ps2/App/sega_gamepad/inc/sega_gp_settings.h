#ifndef SEGA_GAME_PAD_SETTINGS
#define SEGA_GAME_PAD_SETTINGS

/**
 * @file sega_gp_settings.c
 * @author Михаил Каа
 * @brief 
 * 
 * @version 0.1
 * @date 2022-01-22
 * Date: 21.01.2022
 * @copyright Copyright (c) 2021
 */
#define PROC_TIMEOUT    (1024U)
#define PROC_MODEXYZ    (12U)


#define SEGA_WRITE_PIN  HAL_GPIO_WritePin
#define SEGA_READ_PIN   HAL_GPIO_ReadPin
#define SEGA_PIN_L      GPIO_PIN_RESET
#define SEGA_PIN_H      GPIO_PIN_SET

#define SEGA_PIN_1_PORT sega_db9_1_GPIO_Port
#define SEGA_PIN_1      sega_db9_1_Pin
#define SEGA_PIN_2_PORT sega_db9_2_GPIO_Port
#define SEGA_PIN_2      sega_db9_2_Pin
#define SEGA_PIN_3_PORT sega_db9_3_GPIO_Port
#define SEGA_PIN_3      sega_db9_3_Pin
#define SEGA_PIN_4_PORT sega_db9_4_GPIO_Port
#define SEGA_PIN_4      sega_db9_4_Pin
#define SEGA_PIN_6_PORT sega_db9_6_GPIO_Port
#define SEGA_PIN_6      sega_db9_6_Pin
#define SEGA_PIN_9_PORT sega_db9_9_GPIO_Port
#define SEGA_PIN_9      sega_db9_9_Pin
#define SEGA_PIN_7_PORT sega_db9_7_sel_GPIO_Port
#define SEGA_PIN_7      sega_db9_7_sel_Pin

#endif /* SEGA_GAME_PAD_SETTINGS */