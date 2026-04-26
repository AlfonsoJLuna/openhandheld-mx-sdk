#include "MX_LED.h"

#include <stm32l4xx.h>
#include <stm32l4xx_ll_bus.h>
#include <stm32l4xx_ll_gpio.h>


void MX_LED_Init()
{
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);

    LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_1);
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_1, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_1, LL_GPIO_SPEED_FREQ_LOW);
    LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_1, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_1, LL_GPIO_PULL_NO);
    
    LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_8);
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_8, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_8, LL_GPIO_SPEED_FREQ_LOW);
    LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_8, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_8, LL_GPIO_PULL_NO);
    
    LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_11);
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_11, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_11, LL_GPIO_SPEED_FREQ_LOW);
    LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_11, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_11, LL_GPIO_PULL_NO);
}

void MX_LED_On(uint8_t R, uint8_t G, uint8_t B)
{
    if (R)  LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_8);   else    LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_8);
    if (G)  LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_11);  else    LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_11);
    if (B)  LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_1);   else    LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_1);
}

void MX_LED_Off()
{
    MX_LED_On(0, 0, 0);
}
