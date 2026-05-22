#ifndef __UART_Task_h
#define __UART_Task_h

#include "stdio.h"

void UART_Task(void *params);
int fputc(int ch,FILE *F);
#endif
