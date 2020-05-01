/* 
 * File:   I2CCom.h
 * Author: Orlando Lara
 *
 * Created on April 14, 2020, 6:48 PM
 */

#ifndef I2CCOM_H
#define	I2CCOM_H

#include <xc.h>
#include <stdbool.h>
#include "pic18f2331.h"

#define HOLD_BUS() SSPCONbits.CKP =  0
#define RELEASE_BUS() SSPCONbits.CKP =  1
#define RESET_FLAG() SSPIF = 0

enum operation_modes {MASTER, SLAVE};
enum transfer_type {RECEIVED,SENT, IDLE};

void I2CInit(enum operation_modes operation_mode, char address);
unsigned char I2CDataTransfered(unsigned char* i2c_data, unsigned char input_len, unsigned char output_len);
void I2CSend(unsigned char input);
void I2CReceive(unsigned char *msg_input);
bool I2CCheckError(void);
void clearBuffer(void);

#endif	/* I2CCOM_H */

