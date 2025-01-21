/*
 * app.c
 *
 *  Created on: Jan 16, 2025
 *      Author: user
 */

#include "app.h"
#include <stdlib.h>
// 장치 선언
extern TIM_HandleTypeDef htim1; // buzzer
extern TIM_HandleTypeDef htim3; // ultrasonic
extern TIM_HandleTypeDef htim11; // delay_us
extern UART_HandleTypeDef huart2;

// us동안 지연
void delay_us(uint16_t time) {
	htim11.Instance->CNT = 0;
	while(htim11.Instance->CNT < time);
}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void setServo(uint16_t angle) {
	htim1.Instance->CCR1 = map(angle, 0, 180, 800, 2400);
}

void app() {
	initUart(&huart2);
	initUltrasonic(&htim3);
	HAL_TIM_Base_Start(&htim11);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	while(1) {
		// 각도 수신 (AXXX\n) (ack:aXXX\n)
		char* msg = getMessage();
		if (strlen(msg) > 3) {
			if(msg[0] == 'A') {
				// parse
				int angle = atoi(&msg[1]);
				// 서보를 수신 각도로 이동
				setServo(angle);
				// 움직임을 고려
				HAL_Delay(200);
				// 거리 측정
				int distance = getDistance();
				// 측정 데이터 통신 (DXXX\n)
				printf("D%03d\n", distance);
			}
		}
	}
}
