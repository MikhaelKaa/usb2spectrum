#ifndef _SEGA_GAME_PAD
#define _SEGA_GAME_PAD

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

#include "sega_gp_settings.h"
#include "sega_gp.h"

/**
 * @brief 
 * Настройка отладочного вывода.
 * 0U - отключен, 1U - включен
 */
#define SEGA_DEBUG_LEVEL   0U
#if (SEGA_DEBUG_LEVEL == 1U)
#define  SEGA_DBG(...)   do {printf(__VA_ARGS__);} while (0)
#else
#define SEGA_DBG(...) do {} while (0)
#endif

typedef enum sega_phase{
    phase_copy = 0,
    phase_sel_high,
    phase_read_sel_h,
    phase_sel_low,
    phase_read_sel_low,
    phase_mode_pulse1_l,
    phase_mode_pulse1_h,
    phase_mode_pulse2_l,
    phase_mode_pulse2_h,
    phase_mode_pulse3_l,
    phase_mode_pulse3_h,
    phase_read_mxyz,
    phase_comprare,
    phase_proc_timeout,
    
}sega_phase_t;

#endif /* _SEGA_GAME_PAD */