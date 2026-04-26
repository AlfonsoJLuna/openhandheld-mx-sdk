#ifndef MX_ADC_H
#define MX_ADC_H


#include <stdint.h>


void MX_ADC_Init();

int16_t MX_ADC_Joystick_X();

int16_t MX_ADC_Joystick_Y();

uint16_t MX_ADC_VCC_mV();


#endif
