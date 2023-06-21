#ifndef _SPECTRUM_H
#define _SPECTRUM_H

/**
 * @file _spectrum.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-03-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "spectrum.h"
#include <stdio.h>

/**
 * @brief 
 * Настройка отладочного вывода.
 * 0U - отключен, 1U - включен
 */
#define SPECTRUM_DEBUG_LEVEL   1U
#if (SPECTRUM_DEBUG_LEVEL == 1U)
#define  SP_DBG(...)   do {printf(__VA_ARGS__);} while (0)
#else
#define SP_DBG(...) do {} while (0)
#endif

/**
 * @brief 
 * Количество циклов обновления от нажатия до отпускания кнопки.
 */
#define OPT_RESET 7
#define OPT_NMI   2


#endif /* _SPECTRUM_H */
