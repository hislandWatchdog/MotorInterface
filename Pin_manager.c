/*
 * File:   Pin_manager.c
 * Author: juand
 *
 * Created on 9 de marzo de 2020, 06:00 PM
 */


#include <xc.h>
#include "Pin_manager.h"
void PIN_MANAGER_Initialize (void)
{
    LED1_SetDigitalOutput();
    C0_SetDigitalOutput();
    C1_SetDigitalOutput();
    LED1_SetOn();
    
}