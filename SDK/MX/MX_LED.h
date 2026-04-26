#ifndef MX_LED_H
#define MX_LED_H


#include <stdint.h>


void MX_LED_Init();

void MX_LED_On(uint8_t R, uint8_t G, uint8_t B);

void MX_LED_Off();


#endif
