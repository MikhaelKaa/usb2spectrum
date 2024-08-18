/**
 * @file sega_gamepad.c
 * @author Михаил Каа
 * @brief 
 * 
 * @version 0.1
 * @date 2022-01-22
 * Date: 21.01.2022
 * @copyright Copyright (c) 2021
 */

#include "_sega_gp.h"
#include <string.h>
#include <stdio.h>

sega_game_pad_t sega_gpad;
sega_game_pad_t old_data;
sega_phase_t proc_phase = phase_copy;
int proc_timeout = 0;
int mode_timeout = 0;

uint8_t sega_gamepad_proc(void)
{
uint8_t sega_gp_flag = 0;
    switch (proc_phase)
    {
    case phase_copy:
        memcpy(&old_data, &sega_gpad, sizeof(sega_game_pad_t));
        proc_timeout = PROC_TIMEOUT;
        proc_phase = phase_sel_high;
        break;

    case phase_sel_high:
        SEGA_WRITE_PIN(SEGA_PIN_7_PORT, SEGA_PIN_7, SEGA_PIN_H);
        proc_phase = phase_read_sel_h;
        break;

    case phase_read_sel_h:
        sega_gpad.btn_up     = !SEGA_READ_PIN(SEGA_PIN_1_PORT, SEGA_PIN_1);
        sega_gpad.btn_down   = !SEGA_READ_PIN(SEGA_PIN_2_PORT, SEGA_PIN_2);
        sega_gpad.btn_left   = !SEGA_READ_PIN(SEGA_PIN_3_PORT, SEGA_PIN_3);
        sega_gpad.btn_right  = !SEGA_READ_PIN(SEGA_PIN_4_PORT, SEGA_PIN_4);
        sega_gpad.btn_B      = !SEGA_READ_PIN(SEGA_PIN_6_PORT, SEGA_PIN_6);
        sega_gpad.btn_C      = !SEGA_READ_PIN(SEGA_PIN_9_PORT, SEGA_PIN_9);
        proc_phase = phase_sel_low;
        break;

    case phase_sel_low:
        SEGA_WRITE_PIN(SEGA_PIN_7_PORT, SEGA_PIN_7, SEGA_PIN_L);
        proc_phase = phase_read_sel_low;
        break;

    case phase_read_sel_low:
        sega_gpad.btn_A      = !SEGA_READ_PIN(SEGA_PIN_6_PORT, SEGA_PIN_6);
        sega_gpad.btn_start  = !SEGA_READ_PIN(SEGA_PIN_9_PORT, SEGA_PIN_9);
        proc_phase = phase_mode_pulse1_l;
        break;

    case phase_mode_pulse1_l:
        if(mode_timeout--) {
            SEGA_WRITE_PIN(SEGA_PIN_7_PORT, SEGA_PIN_7, SEGA_PIN_L);
            proc_phase = phase_mode_pulse1_l;
        }
        else {
            proc_phase = phase_mode_pulse1_h;
            mode_timeout = PROC_MODEXYZ;
        }
        break;

    case phase_mode_pulse1_h:
        if(mode_timeout--) {
            SEGA_WRITE_PIN(SEGA_PIN_7_PORT, SEGA_PIN_7, SEGA_PIN_H);
            proc_phase = phase_mode_pulse1_h;
        }
        else {
            proc_phase = phase_mode_pulse2_l;
            mode_timeout = PROC_MODEXYZ;
        }
        break;

    case phase_mode_pulse2_l:
        if(mode_timeout--) {
            SEGA_WRITE_PIN(SEGA_PIN_7_PORT, SEGA_PIN_7, SEGA_PIN_L);
            proc_phase = phase_mode_pulse2_l;
        }
        else {
            proc_phase = phase_mode_pulse2_h;
            mode_timeout = PROC_MODEXYZ;
        }
        break;

    case phase_mode_pulse2_h:
        if(mode_timeout--) {
            SEGA_WRITE_PIN(SEGA_PIN_7_PORT, SEGA_PIN_7, SEGA_PIN_H);
            proc_phase = phase_mode_pulse2_h;
        }
        else {
            proc_phase = phase_mode_pulse3_l;
            mode_timeout = PROC_MODEXYZ;
        }
        break;

    case phase_mode_pulse3_l:
        if(mode_timeout--) {
            SEGA_WRITE_PIN(SEGA_PIN_7_PORT, SEGA_PIN_7, SEGA_PIN_L);
            proc_phase = phase_mode_pulse3_l;
        }
        else {
            proc_phase = phase_mode_pulse3_h;
            mode_timeout = PROC_MODEXYZ;
        }
        break;

    case phase_mode_pulse3_h:
        if(mode_timeout--) {
            SEGA_WRITE_PIN(SEGA_PIN_7_PORT, SEGA_PIN_7, SEGA_PIN_H);
            proc_phase = phase_mode_pulse3_h;
        }
        else {
            proc_phase = phase_read_mxyz;
            mode_timeout = PROC_MODEXYZ;
        }
        break;

    case phase_read_mxyz:
        sega_gpad.btn_Z     = !SEGA_READ_PIN(SEGA_PIN_1_PORT, SEGA_PIN_1);
        sega_gpad.btn_Y     = !SEGA_READ_PIN(SEGA_PIN_2_PORT, SEGA_PIN_2);
        sega_gpad.btn_X     = !SEGA_READ_PIN(SEGA_PIN_3_PORT, SEGA_PIN_3);
        sega_gpad.btn_mode  = !SEGA_READ_PIN(SEGA_PIN_4_PORT, SEGA_PIN_4);
        proc_phase = phase_comprare;
        break;

    case phase_comprare:
        sega_gp_flag = memcmp(&old_data, &sega_gpad, sizeof(sega_game_pad_t));
        if(sega_gp_flag) {
            SEGA_DBG("sega gamepad event:\n");
            SEGA_DBG("up\t%d\n",      sega_gpad.btn_up);
            SEGA_DBG("down\t%d\n",    sega_gpad.btn_down);
            SEGA_DBG("left\t%d\n",    sega_gpad.btn_left);
            SEGA_DBG("right\t%d\n",   sega_gpad.btn_right);
            SEGA_DBG("A\t%d\n",       sega_gpad.btn_A);
            SEGA_DBG("B\t%d\n",       sega_gpad.btn_B);
            SEGA_DBG("C\t%d\n",       sega_gpad.btn_C);
            SEGA_DBG("start\t%d\n",   sega_gpad.btn_start);
            SEGA_DBG("X\t%d\n",       sega_gpad.btn_X);
            SEGA_DBG("Y\t%d\n",       sega_gpad.btn_Y);
            SEGA_DBG("Z\t%d\n",       sega_gpad.btn_Z);
            SEGA_DBG("mode\t%d\n\n",  sega_gpad.btn_mode);
        }
        proc_phase = phase_proc_timeout;
        break;

    case phase_proc_timeout:
        if(proc_timeout--) proc_phase = phase_proc_timeout;
        else proc_phase = phase_copy;
        break;

    default:
        proc_phase = phase_copy;
        break;
    }
    return sega_gp_flag;
}