/**
 * @file ps2_keyboard.c
 * @author Михаил Каа
 * @brief
 * Header: Разбор данных от ps2 клавиатуры.
 * @version 0.1
 * @date 28.10.2021
 * @copyright Copyright (c) 2021
 *
 */

#include <stdio.h>
#include <stdint.h>

#include "ps2.h"
#include "ps2_table.h"
#include "spectrum.h"

void ps2_make(uint8_t byte);
void ps2_brake(uint8_t byte);

static queue_t ps2_kb_raw;
static bool flag_f0, flag_e0;
static uint8_t flag_e1;


void ps2_make(uint8_t byte) {
	/*if(flag_e1) {
		epm_5x8_add(0x48);
		return;
	}*/
	if (flag_e0) epm_5x8_add(e0_table_5x8[byte]);
	else epm_5x8_add(main_table_5x8[byte]);
	printf("ps2 make %d (0x%2x)\n", byte, byte);
}

void ps2_brake(uint8_t byte) {
	if (flag_e0) epm_5x8_remove(e0_table_5x8[byte]);
	else epm_5x8_remove(main_table_5x8[byte]);
}


void ps2_proc(void) {
	uint8_t temp;
	while (!queue_empty(&ps2_kb_raw)) {
		temp = queue_get(&ps2_kb_raw);
		if (temp == 0x00) continue;
		switch (temp)
		{
		case 0x00:
			break;

		case 0xF0:
			flag_f0 = true;
			break;

		case 0xE0:
			flag_e0 = true;
			break;

		case 0xE1:
			flag_e1 = 2;
			break;

		default:
			if (flag_f0) {
				ps2_brake(temp);
				flag_f0 = false;
				flag_e0 = false;
				if (flag_e1 != 0) flag_e1--;
				epm_5x8_flush_ps2();
				break;
			}
			ps2_make(temp);
			flag_e0 = false;
			if (flag_e1 != 0) flag_e1--;
			epm_5x8_flush_ps2();
			break;
		}
	}
}

void ps2_put(uint8_t byte) {
	queue_put(&ps2_kb_raw, byte);
	//queue_put_wo_duplicates(&ps2_kb_raw, byte);
}

void ps2_init(void) {
	queue_init(&ps2_kb_raw);
}