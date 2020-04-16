/* 
 * File:   I2CCom.c
 * Author: Orlando Lara
 *
 * Created on April 14, 2020, 6:48 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "I2CCom.h"

void I2CInit(enum operation_modes operation_mode, char address){
    if(operation_mode == SLAVE){    //Slave mode
        /*
         * The SSP module will override the input state with the output
         * data when required (slave-transmitter)
         */
        TRISC5 = 1;     //SCL as input
        TRISC4 = 1;     //SDA as input
        
        SSPSTATbits.SMP = 0;
        SSPSTATbits.CKE = 0;
        
        //There is no need to setup interrupts on start and stop bits because
        //an interrupt will be generated on every data byte transmitted or received
        SSPCON = 0b00110110;       //reset everything and set I2C slave mode 7-bit address
        //There is no automatic clock stretching in this PIC
        
        SSPADD = address << 1;      //SSPADD<0> is assumed to be 0, remember it is 7-bit addressing
        
        SSPIF = 0;
        SSPIE = 1;  //Interrupt enable bit
        SSPIP = 1;  //High priority interruption
    }
    else if(operation_mode == MASTER){
        Nop();
    }
}

unsigned char I2CDataTransfered(unsigned char* i2c_data, unsigned char input_len, unsigned char output_len){
    /*
     * I should mention that this clock stretching was done explicitely because it isn't
     * automatic like in PIC18F25K80. Also note that since clock stretching is done
     * automatically in PIC18F25K80 for reading, I cannot do this explicit clock
     * stretching. For this pic, there seems to be no problem
    */
    HOLD_BUS();
    static unsigned char index;
    
    unsigned char key;
    unsigned char process_done = 0;
        
    if(I2CCheckError())
        return process_done;
    
    key = (SSPSTATbits.D_NOT_A << 1) + SSPSTATbits.R_NOT_W;
    
    switch(key){
        case 0:                     //Last byte was ADDRESS - WRITE
            index = 0;
            
            clearBuffer();
            RELEASE_BUS();
            break;
        
        case 1:                     //Last byte was ADDRESS - READ
            index = 0;
            clearBuffer();          //Read address match byte
            
            I2CSend(*(i2c_data + index++));
            break;
            
        case 2:                     //Last byte was DATA    - WRITE
            if(index < input_len){
                I2CReceive(i2c_data + index++);
                if(index == input_len)
                    process_done = RECEIVED;
            }
            else{
                /*
                 * received some data that needs to be
                 * cleared from the buffer but don't want to
                 * save it anywhere
                */
                clearBuffer();
                RELEASE_BUS();
            }
            break;
            
        case 3:                     //Last byte was DATA    - READ
            if(index < output_len){
                I2CSend(*(i2c_data + index++));
                if(index == output_len)
                    process_done = SENT;
            }
            else{
                /*
                 *Asked to send more data but no more data is available
                 * sending 0
                */
                I2CSend(255);
            }
            break;
            
        default:
            break;
    }
    RELEASE_BUS();
    return process_done;
}

void I2CSend(unsigned char input){
    SSPBUF = input;             //load data
    RELEASE_BUS();              //SCL pin is enabled (clock is released)
    while(SSPSTATbits.BF);      //Wait for buffer to be empty
}

void I2CReceive(unsigned char *msg_input){
    *msg_input = SSPBUF;
    RELEASE_BUS();              //SCL pin is enabled (clock is released)
    while(SSPSTATbits.BF);      //Wait for buffer to be empty
}

bool I2CCheckError(void){
    bool error_detected = false;
    
    if(SSPCONbits.SSPOV){
        SSPCONbits.SSPOV = 0;      //clear error by software
        clearBuffer();             //clear BF bit by reading SSPBUF
        error_detected = true;
    }
    if(SSPCONbits.WCOL){
        SSPCONbits.WCOL = 0;       //write collision occurred
        clearBuffer();             //clear BF bit by reading SSPBUF
        error_detected = true;
    }
    return error_detected;
}

void clearBuffer(void){
    char trash;
    trash = SSPBUF;             //BF bit must be cleared
    while(SSPSTATbits.BF);      //Wait for buffer to be empty
}
