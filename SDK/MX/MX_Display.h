#ifndef MX_Display_H
#define MX_Display_H


#include <stdint.h>


void MX_Display_Init();

void MX_Display_Set_Window(uint16_t XS, uint16_t XE, uint16_t YS, uint16_t YE);

void MX_Display_Send_Pixel(uint16_t RGB);


#endif
