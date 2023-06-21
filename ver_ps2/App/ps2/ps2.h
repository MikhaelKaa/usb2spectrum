#ifndef PS2_H
#define PS2_H

#include <stdio.h>
#include <stdint.h>

#include "queue.h"

// Инициализация.
void ps2_init();

// В прерывании SPI закидываем байты от клавиатуры.
void ps2_put(uint8_t byte);

// Обработка накопленных данных.
void ps2_proc();

#endif /* PS2_H */
