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

#define PIC1_ADDR 0x1B
#define input_len 4
#define output_len 4

typedef struct{
    unsigned char age;
    unsigned char brothers;
    unsigned char height;
    unsigned char weight;
}person;

person billy;
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

void fillBuffer(person *obj){
    buffer[0] = ++obj->age;
    buffer[1] = ++obj->brothers;
    buffer[2] = ++obj->height;
    buffer[3] = ++obj->weight;
}

void __interrupt(high_priority) isr_high(void){
    if(SSPIE && SSPIF){
        SSPIF = 0;
        char data_state = I2CDataTransfered(buffer,input_len, output_len);
        
        switch(data_state){
            case RECEIVED:
                billy.age       = buffer[0];
                billy.brothers  = buffer[1];
                billy.height    = buffer[2];
                billy.weight    = buffer[3];
                
                fillBuffer(&billy);
                break;
            case SENT:
                break;
            default:
                break;
        }
    }
}

void main(void) {
    IPEN   = 1;       //Enable interrupt priorities
    GIEH   = 1;       //Enable High priority interruptions
    
    resetPorts();
    I2CInit(SLAVE, PIC1_ADDR);
    
    memset(&billy,0,sizeof(billy));     //Clear all information in data structure
    
    PORTCbits.RC3 = 0; //LED ON
    while(1){
    }
}

