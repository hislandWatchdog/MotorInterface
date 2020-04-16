/*
 * File:   qei.c
 * Author: juand
 *
 * Created on September 26, 2019, 10:38 PM
 */


#include <xc.h>
#include "QEI.h"

void Quadrature_Encoder_Initialize(void)
{
    ANS3 = 0;
    ANS4 = 0;
    TRISA3 = 1;
    TRISA4 = 1;
    QEICON = 0b00011011;
    T5CON = 0b00000001;
    DFLTCON = 0b01110000;   //Digital Filter Control Register
    CAP1CON = 0b01000010;
    //CAP1REN = 1;
}