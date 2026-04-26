#include "MX_ADC.h"

#include <stm32l4xx_ll_bus.h>
#include <stm32l4xx_ll_rcc.h>
#include <stm32l4xx_ll_gpio.h>
#include <stm32l4xx_ll_adc.h>


static void Setup_GPIO()
{
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);

    LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_0, LL_GPIO_SPEED_FREQ_LOW);
    LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_0, LL_GPIO_PULL_NO);
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_0, LL_GPIO_MODE_ANALOG);
    
    LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_3, LL_GPIO_SPEED_FREQ_LOW);
    LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_3, LL_GPIO_PULL_NO);
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_3, LL_GPIO_MODE_ANALOG);
    
    LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_4, LL_GPIO_SPEED_FREQ_LOW);
    LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_4, LL_GPIO_PULL_NO);
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_4, LL_GPIO_MODE_ANALOG);
}

static void Setup_ADC()
{
    LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSOURCE_SYSCLK);
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_ADC);

    LL_ADC_InitTypeDef Config;
    LL_ADC_StructInit(&Config);
    LL_ADC_Init(ADC1, &Config);

    LL_ADC_REG_InitTypeDef RegConfig;
    LL_ADC_REG_StructInit(&RegConfig);
    LL_ADC_REG_Init(ADC1, &RegConfig);

    LL_ADC_REG_SetSequencerLength(ADC1, LL_ADC_REG_SEQ_SCAN_DISABLE);
    LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_SINGLE);

    LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_15, LL_ADC_SAMPLINGTIME_247CYCLES_5); // VCC_DET
    LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_7, LL_ADC_SAMPLINGTIME_247CYCLES_5); // JOY_Y
    LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_9, LL_ADC_SAMPLINGTIME_247CYCLES_5); // JOY_X

    LL_ADC_StartCalibration(ADC1, LL_ADC_SINGLE_ENDED);
    while (LL_ADC_IsCalibrationOnGoing(ADC1));

    LL_ADC_ClearFlag_ADRDY(ADC1);
}

void MX_ADC_Init()
{
    Setup_GPIO();
    Setup_ADC();
}

static uint16_t Read_ADC()
{
    LL_ADC_Enable(ADC1);
    while (!LL_ADC_IsEnabled(ADC1));

    LL_ADC_REG_StartConversion(ADC1);
    while (LL_ADC_REG_IsConversionOngoing(ADC1));

    uint16_t Value = LL_ADC_REG_ReadConversionData12(ADC1);

    LL_ADC_REG_StopConversion(ADC1);
    LL_ADC_Disable(ADC1);

    return Value;
}

int16_t MX_ADC_Joystick_X()
{
    LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_9);
    return (int16_t)Read_ADC() - 2048;
}

int16_t MX_ADC_Joystick_Y()
{
    LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_7);
    return (int16_t)Read_ADC() - 2048;
}

uint16_t MX_ADC_VCC_mV()
{
    LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_15);
    return Read_ADC();
}
