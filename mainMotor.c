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


#define USING_RIGHT_MOTOR true
#define PIC1_ADDR 0x1D
#define PIC2_ADDR 0x1B

#define INPUT_BYTES 3
#define OUTPUT_BYTES 3
#define BUF_LEN (INPUT_BYTES < OUTPUT_BYTES) ? OUTPUT_BYTES : INPUT_BYTES

#define PIC_ADDR USING_RIGHT_MOTOR ? PIC1_ADDR : PIC2_ADDR
#define QEI_MOTOR_DIRECTION USING_RIGHT_MOTOR ? QEICONbits.UP_nDOWN : !QEICONbits.UP_nDOWN

#define STOP_QEI() IC1IE = 0
#define START_QEI() IC1IE = 1

unsigned char buffer[BUF_LEN] = {0};

unsigned int rpm = 0;
unsigned int rpm_decimals = 0;


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
        unsigned char data_state = I2CDataTransfered(buffer,INPUT_BYTES, OUTPUT_BYTES);
        
        switch(data_state){
            case RECEIVED:
                //duty_received[0] = buffer[0];
                //duty_received[1] = buffer[1];
                PWM_Set_Duty(buffer[0]+ ((unsigned int)buffer[1] <<8) );
                Motor_Direction(buffer[2]); 
                buffer[0] = rpm;
                buffer[1] = rpm_decimals;
                buffer[2] = QEI_MOTOR_DIRECTION;
                break;
            case SENT:
                break;
            default:
                break;
        }
    }
}

void __interrupt(low_priority) isr_low(void){
     if(IC1IF == 1 && IC1IE == 1){
        IC1IF = 0; 
        
        unsigned int qei_pulse_period;
        
        qei_pulse_period = ((unsigned int)(VELRH<<8)) | (unsigned int)VELRL;
        rpm = CONSTANT_VELOCITY/((unsigned long)qei_pulse_period);
        rpm_decimals = (((CONSTANT_VELOCITY) %((unsigned long)qei_pulse_period))*100)/qei_pulse_period;
    }
}

void main(void) {
    IPEN = 1;       //Enable interrupt priorities
    GIEH = 1;       //Enable High priority interruptions
    GIEL = 1;       //Enable Low priority interruptions   
    
    resetPorts();
    
    I2CInit(SLAVE, PIC_ADDR);
    PWM_Initialize();
    Quadrature_Encoder_Initialize(LOW_PRIORITY);
    PIN_MANAGER_Initialize();
    Motor_Direction(STOP_MOTOR); 
    
    PWM_Set_Duty(0); //70-1000
    
    LED1_SetOn();
    while(1){
    }
}

