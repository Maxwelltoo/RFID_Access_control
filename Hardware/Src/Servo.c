/*
 * Servo.c
 *  mg90s servo control
 *  Created on: Mar 2, 2021
 *      Author: Logos
 */
#include "main.h"
#include "Servo.h"
#include "tim.h"

void OpenDoor()
{
	Servo_Control(160);
    HAL_Delay(3200);
    Servo_Control(50);
    HAL_Delay(100);
}

// angle:角度值，0~180
void Servo_Control(uint16_t angle)
{
   float temp; //占空比值
   temp =(1.0 / 9.0) * angle + 5.0; // 5~25（周期200）
   __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, (uint16_t )temp);
}
