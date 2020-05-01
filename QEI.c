/*
 * File:   qei.c
 * Author: juand
 *
 * Created on September 26, 2019, 10:38 PM
 */


#include <xc.h>
#include "QEI.h"

void Quadrature_Encoder_Initialize(enum priority_level priority)
{
    ANS3 = 0;
    ANS4 = 0;
    
    TRISA3 = 1;
    TRISA4 = 1;
    
    QEICON = 0b00011001;
    T5CON = 0b00000001;
    DFLTCON = 0b01110000;   //Digital Filter Control Register
    CAP1CON = 0b01000100;
    CAP1REN = 1;
    
    switch(priority){
        case NO_INTERRUPTIONS:
            IC1IE  = 0;       //Disable IC1 Interruption
            break;
        case HIGH_PRIORITY:
            IC1IE  = 1;       //Enable IC1 Interruption
            IC1IP  = 1;       //Set IC1 as High Priority
            break;
        case LOW_PRIORITY:
            IC1IE  = 1;       //Enable IC1 Interruption
            IC1IP  = 0;       //Set IC1 as Low Priority
            break;
        default:
            //do nothing
            break;
    }
}
