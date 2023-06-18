//01.11.2020
#include <stdio.h>
#include <string.h>

#include "main.h"
#include "retarget.h"
//#include "usbd_cdc_if.h"

// printf buffer
#define BUF_SZ  1024
uint8_t  buf[BUF_SZ];
volatile int counter = 0;

// uart на отладочной плате
extern UART_HandleTypeDef huart2;
//extern UART_HandleTypeDef huart2;


// Circular update
void printf_flush(void)
{
  if(counter)
  {
    int size = counter;
    if(HAL_BUSY == HAL_UART_Transmit_DMA(&huart2, buf, size)) return;
    counter = 0;
  }
}

int sendchar (int c)
{
  buf[counter++%BUF_SZ] = c;
  //ITM_SendChar(c);
  return(c);
}

int _write(int fd, char* ptr, int len)
{
    (void)fd;
    int i = 0;
    while (ptr[i] && (i < len)) {
        sendchar((int)ptr[i++]);
    }
    return len;
}
