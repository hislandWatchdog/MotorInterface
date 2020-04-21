/*
 * File:   mainWatchdog.c
 * Authors: Orlando Lara, Juan de Dios Durán
 *
 * Created on April 14, 2020, 6:45 PM
 */

#pragma config OSC = HS         // Oscillator Selection bits (HS oscillator)
#pragma config WDTEN = OFF      // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config MCLRE = OFF       // MCLR Pin Enable bit (Enabled)

#include <xc.h>
#include <stdio.h>
#include <string.h>
#include "pic18f2331.h"
#include "I2CCom.h"
#include "PWM.h"
#include "QEI.h"
#include "Pin_manager.h"


#define PIC1_ADDR 0x1D
#define input_len 3
#define output_len 3

#define STOP_QEI() IC1IE = 0
#define START_QEI() IC1IE = 1

unsigned char buffer[(input_len < output_len ? output_len : input_len)] = {0};

unsigned int counter = 0;
unsigned int rpm = 0;

void resetPorts(void){
    //Make sure that all unused IO pins are set to output and cleared
    TRISA = 0x00;
    TRISB = 0x00;
    TRISC = 0x00;
    
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
}


void __interrupt(high_priority) isr_high(void){
    if(SSPIE && SSPIF){
        STOP_QEI();
        unsigned char data_state = I2CDataTransfered(buffer,input_len, output_len);
        
        switch(data_state){
            case RECEIVED:
                buffer[0] = rpm;
                buffer[1]++;
                buffer[2]++;
                break;
            case SENT:
                START_QEI();
                break;
            default:
                break;
        }
    }
    
    if(IC1IF == 1 && IC1IE == 1){
        IC1IF = 0; 
        
        counter = ((unsigned int)(VELRH<<8)) | (unsigned int)VELRL;
        rpm = CONSTANT_VELOCITY/((unsigned long)counter);
    }
}

void main(void) {
    IPEN   = 1;       //Enable interrupt priorities
    GIEH   = 1;       //Enable High priority interruptions
    IC1IE  = 1;       //Enable IC1 Interruption
    IC1IP  = 1;       //Set IC1 as High Priority
    
    //resetPorts();
    I2CInit(SLAVE, PIC1_ADDR);
    
    
    PWM_Initialize();
    Quadrature_Encoder_Initialize();
    PIN_MANAGER_Initialize();
    
    PWM_Set_Duty(500);
    
    PORTCbits.RC6 = 0; //LED ON
    while(1){
        //LED1_Toggle();
    }
}

