/*
 * ultrasonic.h
 *
 *  Created on: Jan 18, 2025
 *      Author: user
 */

#ifndef INC_ULTRASONIC_H_
#define INC_ULTRASONIC_H_

#include "main.h"

// prototype define
void initUltrasonic(TIM_HandleTypeDef *inHtim);
uint16_t getDistance();

#endif /* INC_ULTRASONIC_H_ */
