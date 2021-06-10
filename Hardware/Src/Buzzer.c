/*
 * Buzzer.c
 *
 *  Created on: 2021年3月3日
 *      Author: Logos
 */
#include "main.h"
#include "Buzzer.h"
#include "tim.h"

/**
  * @brief 蜂鸣器 音量
  * @param 音量
  * @note  音量1~8
  * @retval None
  */
void Sound(unsigned char volume)
{
	unsigned short Autoreload;
	Autoreload=__HAL_TIM_GET_AUTORELOAD(&htim3);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, Autoreload>>(10-volume)); // 设置音量：比较值
}


/**
  * @brief 蜂鸣器 音调
  * @param 频率（音调宏定义）
  * @note  频率10~20000
  * @retval None
  */
void Tone(unsigned short frequency)
{
	unsigned short Autoreload;
    if((frequency<10)||(frequency>20000))                // 发声频率范围
    	Mute();
    else
    {
        Autoreload=(1000000/frequency)-1;                               //计算自动重装值，定时器3分频后1Mhz
        __HAL_TIM_SET_AUTORELOAD(&htim3, Autoreload);                     // 设置频率：自动重装值
        __HAL_TIM_SET_COUNTER(&htim3, 0);                                 // 计数值清零
    }
}

void Mute(void) // 不发音
{
	__HAL_TIM_SET_COUNTER(&htim3, 0);                // 计数值清零
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0); // 设置音量：比较值
}

void Beat(unsigned short note)
{
	unsigned short time; // 四分音符1/4s
	time = 1000/note;
	HAL_Delay(time);
}

void Beep_Start(void)
{
	Sound(5);
	Tone(M5); Beat(2);
	Tone(M4); Beat(4);
	Tone(H1); Beat(1);
	Mute();
}

void Beep_Welcome(void)
{
	Sound(6);
	Tone(H4); Beat(8);
	Mute();
}

void Beep_Error(void)
{
	Sound(6);
	Tone(L1); Beat(8);
	Mute();   Beat(8);
	Sound(6); Beat(8);
	Mute();
}
