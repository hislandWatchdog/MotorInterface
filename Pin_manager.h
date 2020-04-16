/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File: pin_manager.h
 * Author: Juan de Dios Duuran
 * Comments:
 * Revision history: V1.0
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef PIN_MANAGER_H
#define	PIN_MANAGER_H

#include <xc.h> // include processor files - each processor file is guarded.  
#define LED1_SetOn()            LATC6 = 0
#define LED1_SetOff()           LATC6 = 1
#define LED1_Toggle()           LATC6 ^= 1
#define LED1_SetDigitalInput()  TRISC6 = 1
#define LED1_SetDigitalOutput() TRISC6 = 0

/*#define LED1_SetOn()            LATC3 = 0
#define LED1_SetOff()           LATC3 = 1
#define LED1_Toggle()           LATC3 ^= 1
#define LED1_SetDigitalInput()  TRISC3 = 1
#define LED1_SetDigitalOutput() TRISC3 = 0*/

#define C0_SetHigh()          LATC0 = 1
#define C0_SetLow()           LATC0 = 0
#define C0_SetDigitalInput()  TRISC0 = 1
#define C0_SetDigitalOutput() TRISC0 = 0

#define C1_SetHigh()          LATC1 = 1
#define C1_SetLow()           LATC1 = 0
#define C1_SetDigitalInput()  TRISC1 = 1
#define C1_SetDigitalOutput() TRISC1 = 0

void PIN_MANAGER_Initialize (void);
void Motor_Direction(char direction);

#endif	/* PIN_MANAGER_H */

