#ifndef SEGA_GAME_PAD
#define SEGA_GAME_PAD

/**
 * @file sega_gp.h
 * @author Михаил Каа
 * @brief 
 * 
 * @version 0.1
 * @date 2022-01-22
 * Date: 21.01.2022
 * @copyright Copyright (c) 2021
 */

#include "main.h"
#include <stdbool.h>

uint8_t sega_gamepad_proc(void);

typedef struct sega_game_pad {
    bool btn_up;
    bool btn_down;
    bool btn_left;
    bool btn_right;
    bool btn_A;
    bool btn_B;
    bool btn_C;
    bool btn_start;
    bool btn_X;
    bool btn_Y;
    bool btn_Z;
    bool btn_mode;
}sega_game_pad_t;

extern sega_game_pad_t sega_gpad;

#endif /* SEGA_GAME_PAD */