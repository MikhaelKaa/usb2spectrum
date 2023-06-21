#ifndef QUEUE_H
#define QUEUE_H

// Найдено на просторах интернетов в открытых источниках.

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define QCAPACITY 128


typedef struct queue
{
	uint8_t container[QCAPACITY];
	int head;
	int tail;
	size_t size;
}queue_t;

void	queue_init(queue_t *q);
void	queue_put_wo_duplicates(queue_t* q, uint8_t elem);
void	queue_put(queue_t *q, uint8_t elem);
uint8_t queue_get(queue_t *q);

bool	queue_empty(queue_t *q);
bool	queue_full(queue_t *q);

#endif
