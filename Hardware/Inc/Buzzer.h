/*
 * Buzzer.h
 *
 *  Created on: Mar 3, 2021
 *      Author: Logos
 */

#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_

// C调音符宏定义
#define L1  262
#define L1D 277
#define L2  294
#define L2D 311
#define L3  330
#define L4  349
#define L4D 370
#define L5  392
#define L5D 415
#define L6  440
#define L6D 466
#define L7  494

#define M1  523
#define M1D 554
#define M2  587
#define M2D 622
#define M3  659
#define M4  698
#define M4D 740
#define M5  784
#define M5D 831
#define M6  880
#define M6D 932
#define M7  988

#define H1  1046
#define H1D 1109
#define H2  1175
#define H2D 1245
#define H3  1318
#define H4  1397
#define H4D 1480
#define H5  1568
#define H5D 1661
#define H6  1760
#define H6D 1865
#define H7  1976

extern void Sound(unsigned char volume);
extern void Tone(unsigned short frequency);
extern void Beat(unsigned short note);
extern void Mute(void);
extern void Beep_Start(void);
extern void Beep_Welcome(void);
extern void Beep_Error(void);
#endif /* INC_BUZZER_H_ */
