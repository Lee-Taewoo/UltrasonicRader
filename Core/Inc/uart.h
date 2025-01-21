/*
 * uart.h
 *
 *  Created on: Jan 16, 2025
 *      Author: user
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "main.h"
#include <stdio.h>
#include <string.h>

void initUart(UART_HandleTypeDef *inuart);
char getUart();
char* getMessage();

#endif /* INC_UART_H_ */
