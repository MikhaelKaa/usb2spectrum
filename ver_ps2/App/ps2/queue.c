// Найдено на просторах интернетов в открытых источниках.
#include "queue.h"

bool queue_empty(queue_t *q)
{
  return q->size == 0;
}

bool queue_full(queue_t *q)
{
  return q->size == QCAPACITY;
}

void queue_init(queue_t *q)
{
  q->head = QCAPACITY - 1;
  q->tail = QCAPACITY - 1;
}

void queue_put(queue_t *q, uint8_t elem)
{
  if (q->size < QCAPACITY)
  {
    q->container[q->head--] = elem;
    q->size++;
    if (q->head < 0)
    {
      q->head = QCAPACITY-1;
    }
  }
}


void queue_put_wo_duplicates(queue_t* q, uint8_t elem)
{
    if (q->size < QCAPACITY)
    {
        if ((q->head != QCAPACITY - 1) && (q->container[q->head + 1] == elem)) {
            //printf("duplicate 0x%02x drop!\n", elem);
            return;
        }

        q->container[q->head--] = elem;
        //printf("added 0x%02x !\n", elem);
        q->size++;
        if (q->head < 0)
        {
            q->head = QCAPACITY - 1;
        }
    }
}


uint8_t queue_get(queue_t *q)
{
  uint8_t out = 0;

  if (q->size > 0)
  {
    out = q->container[q->tail--];
    q->size--;
    if (q->tail < 0)
    {
      q->tail = QCAPACITY-1;
    }
  }
  return out;
}
