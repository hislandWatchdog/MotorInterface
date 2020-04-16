/*
 * File:   pwm.c
 * Author: juand
 *
 * Created on September 26, 2019, 6:45 PM
 */


#include <xc.h>
#include "PWM.h"

void PWM_Initialize(void)
{
    PR2 = 0xFF; // 255 load with PWM period value (fixed at 3.906KHz kHz) 
    CCPR1L = 0x00; // eight high bits of duty cycle 
    TRISCbits.TRISC2 = 0;
    //CCPTMRS = 0x00; //select timer resources, Timer2 for CCP1 and all others, page 208 
    T2CON = 0b00000101; // Timer2 On, 1:1 Post, 4x prescale 
    CCP1CON = 0b00001100; // setup for PWM mode 5:4 are PWM Duty Cycle LSB
}

void PWM_Set_Duty(unsigned int duty) 
{
    CCPR1L = (duty >> 2) & 0b0000000011111111;
    CCP1CONbits.DC1B1 = (duty >> 1) & 0b0000000000000001;
    CCP1CONbits.DC1B0 = duty & 0b0000000000000001;
}