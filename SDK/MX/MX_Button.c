#include "MX_Button.h"

#include <stm32l4xx_ll_bus.h>
#include <stm32l4xx_ll_gpio.h>


void MX_Button_Init()
{
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOH);

    LL_GPIO_SetPinMode(GPIOH, LL_GPIO_PIN_1, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinSpeed(GPIOH, LL_GPIO_PIN_1, LL_GPIO_SPEED_FREQ_LOW);
    LL_GPIO_SetPinPull(GPIOH, LL_GPIO_PIN_1, LL_GPIO_PULL_NO);

    LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_15, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinSpeed(GPIOC, LL_GPIO_PIN_15, LL_GPIO_SPEED_FREQ_LOW);
    LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_15, LL_GPIO_PULL_NO);

    LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_13, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinSpeed(GPIOC, LL_GPIO_PIN_13, LL_GPIO_SPEED_FREQ_LOW);
    LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_13, LL_GPIO_PULL_NO);

    LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_14, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinSpeed(GPIOC, LL_GPIO_PIN_14, LL_GPIO_SPEED_FREQ_LOW);
    LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_14, LL_GPIO_PULL_NO);
}

bool MX_Button_SELECT()
{
    return LL_GPIO_IsInputPinSet(GPIOH, LL_GPIO_PIN_1);
}

bool MX_Button_START()
{
    return LL_GPIO_IsInputPinSet(GPIOC, LL_GPIO_PIN_15);
}

bool MX_Button_B()
{
    return LL_GPIO_IsInputPinSet(GPIOC, LL_GPIO_PIN_13);
}

bool MX_Button_A()
{
    return LL_GPIO_IsInputPinSet(GPIOC, LL_GPIO_PIN_14);
}
