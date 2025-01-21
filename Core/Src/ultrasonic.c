/*
 * ultrasonic.c
 *
 *  Created on: Jan 18, 2025
 *      Author: user
 */

#include "ultrasonic.h"
#include <stdbool.h>

uint16_t IC_VAL1; // 상승 엣지 검출 시간
uint16_t IC_VAL2; // 하강 엣지 검출 시간
uint16_t difference; // IC_VAL2 - IC_VAL1
uint16_t distance; // 거리값 저장
bool isFirstCapture;

TIM_HandleTypeDef *myHtim;

//tim3 interrupt callback
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
		if(isFirstCapture == true) {
			// IC_VAL1에 상승 엣지 시간 저장
			IC_VAL1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
			// 하강 엣지 검출 설정
			isFirstCapture = false;
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
		}
		else {
			// IC_VAL2 하강 엣지 시간 저장
			IC_VAL2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
			if (IC_VAL2 > IC_VAL1) {
				difference = IC_VAL2-IC_VAL1;
			}
			else {
				difference = (65535 - IC_VAL1) + IC_VAL2;
			}
			// 거리 계산
			distance = (uint16_t) ((float)difference * 0.034 / 2);
			// 다음 캡처를 위한 설정
			isFirstCapture = true;
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
			// 인터럽트 끄기
			__HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);
		}
	}
}

void initUltrasonic(TIM_HandleTypeDef *inHtim) {
	myHtim = inHtim;
	HAL_TIM_IC_Start_IT(inHtim, TIM_CHANNEL_1);
}

uint16_t getDistance() {
	//10us 펄스 생성
	HAL_GPIO_WritePin(Trigger_GPIO_Port, Trigger_Pin, 1);
	delay_us(10);
	HAL_GPIO_WritePin(Trigger_GPIO_Port, Trigger_Pin, 0);
	__HAL_TIM_ENABLE_IT(myHtim, TIM_IT_CC1);
	HAL_Delay(66);
	return distance;
}
