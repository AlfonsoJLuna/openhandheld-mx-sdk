#include "MX_Display.h"

#include <stm32l4xx.h>
#include <stm32l4xx_ll_bus.h>
#include <stm32l4xx_ll_gpio.h>
#include <stm32l4xx_ll_spi.h>
#include <stm32l4xx_ll_utils.h>


#define ST7789_CMD_SWRESET 0x01
#define ST7789_CMD_SLPOUT  0x11
#define ST7789_CMD_PTLON   0x12
#define ST7789_CMD_NORON   0x13
#define ST7789_CMD_INVON   0x21
#define ST7789_CMD_DISPOFF 0x28
#define ST7789_CMD_DISPON  0x29
#define ST7789_CMD_PTLAR   0x30
#define ST7789_CMD_CASET   0x2A
#define ST7789_CMD_RASET   0x2B
#define ST7789_CMD_RAMWR   0x2C
#define ST7789_CMD_MADCTL  0x36
#define ST7789_CMD_COLMOD  0x3A
#define ST7789_CMD_WRMEMC  0x3C


static void Setup_GPIO()
{
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);

    // PB8 LCD_LED
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_8, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_8, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_8, LL_GPIO_SPEED_FREQ_LOW);
    LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_8, LL_GPIO_PULL_NO);
    LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_8);

    // PB4 LCD_RESET
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_4, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_4, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_4, LL_GPIO_SPEED_FREQ_LOW);
    LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_4, LL_GPIO_PULL_NO);
    LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_4);

    // PA12 LCD_DC
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_12, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_12, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_12, LL_GPIO_SPEED_FREQ_LOW);
    LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_12, LL_GPIO_PULL_NO);
    LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_12);

    // PB3 LCD_SCK
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_3, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetAFPin_0_7(GPIOB, LL_GPIO_PIN_3, LL_GPIO_AF_5);
    LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_3, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_3, LL_GPIO_PULL_DOWN);

    // PB5 LCD_MOSI
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_5, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetAFPin_0_7(GPIOB, LL_GPIO_PIN_5, LL_GPIO_AF_5);
    LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_5, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_5, LL_GPIO_PULL_DOWN);
}

static void Setup_SPI()
{
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);

    LL_SPI_SetBaudRatePrescaler(SPI1, LL_SPI_BAUDRATEPRESCALER_DIV2);
    LL_SPI_SetTransferDirection(SPI1, LL_SPI_HALF_DUPLEX_TX);
    LL_SPI_SetClockPhase(SPI1, LL_SPI_PHASE_2EDGE);
    LL_SPI_SetClockPolarity(SPI1, LL_SPI_POLARITY_HIGH);
    LL_SPI_SetTransferBitOrder(SPI1, LL_SPI_MSB_FIRST);
    LL_SPI_SetDataWidth(SPI1, LL_SPI_DATAWIDTH_8BIT);
    LL_SPI_SetNSSMode(SPI1, LL_SPI_NSS_SOFT);
    LL_SPI_SetMode(SPI1, LL_SPI_MODE_MASTER);
    LL_SPI_DisableCRC(SPI1);

    LL_SPI_Enable(SPI1);
}

static void SPI_Set_Datawidth_8()
{
    while (LL_SPI_IsActiveFlag_BSY(SPI1));

    LL_SPI_SetDataWidth(SPI1, LL_SPI_DATAWIDTH_8BIT);
}

static void SPI_Set_Datawidth_12()
{
    while (LL_SPI_IsActiveFlag_BSY(SPI1));
    
    LL_SPI_SetDataWidth(SPI1, LL_SPI_DATAWIDTH_12BIT);
}

static void SPI_Transmit_8(uint8_t Tx)
{
    while (!LL_SPI_IsActiveFlag_TXE(SPI1));

    LL_SPI_TransmitData8(SPI1, Tx);
}

static void SPI_Transmit_12(uint16_t Tx)
{
    while (!LL_SPI_IsActiveFlag_TXE(SPI1));

    LL_SPI_TransmitData16(SPI1, Tx);
}

static void SPI_Set_Mode_Command()
{
    SPI_Set_Datawidth_8();

    // Set PA12 LCD_DC to LOW for command mode
    LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_12); 
    LL_mDelay(1);
}

static void SPI_Set_Mode_Param()
{
    SPI_Set_Datawidth_8();

    // Set PA12 LCD_DC to HIGH for data mode
    LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_12);
    LL_mDelay(1);
}

static void SPI_Set_Mode_Data()
{
    SPI_Set_Datawidth_12();

    // Set PA12 LCD_DC to HIGH for data mode
    LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_12);
    LL_mDelay(1);
}

static void ST7789_Send_Command(uint8_t Command)
{
    SPI_Set_Mode_Command();

    SPI_Transmit_8(Command);
    LL_mDelay(1);
}

static void ST7789_Send_Command_Param(uint8_t Command, uint8_t* Param, uint32_t ParamSize)
{
    SPI_Set_Mode_Command();

    SPI_Transmit_8(Command);
    LL_mDelay(1);

    SPI_Set_Mode_Param();

    for (uint32_t i = 0; i < ParamSize; i++)
    {
        SPI_Transmit_8(Param[i]);
    }
}

static void ST7789_HardReset()
{
    LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_4);
    LL_mDelay(50);
    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_4);
    LL_mDelay(50);
    LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_4);
    LL_mDelay(50);
}

void MX_Display_Set_Window(uint16_t XS, uint16_t XE, uint16_t YS, uint16_t YE)
{
    {
        uint8_t Param[] = {XS >> 8, XS & 0xFF, XE >> 8, XE & 0xFF};
        ST7789_Send_Command_Param(ST7789_CMD_CASET, Param, sizeof(Param));
    }
    {
        uint8_t Param[] = {YS >> 8, YS & 0xFF, YE >> 8, YE & 0xFF};
        ST7789_Send_Command_Param(ST7789_CMD_RASET, Param, sizeof(Param));
    }

    ST7789_Send_Command(ST7789_CMD_RAMWR);

    SPI_Set_Mode_Data();
}

void MX_Display_Send_Pixel(uint16_t RGB)
{
    SPI_Transmit_12(RGB);
}

static void ST7789_Init_Memory()
{
    MX_Display_Set_Window(0, 239, 0, 319);

    for (int i = 0; i < 240*320; i++)
    {
        MX_Display_Send_Pixel(0x000);
    }
}

static void Setup_ST7789()
{
    ST7789_HardReset();

    ST7789_Send_Command(ST7789_CMD_SWRESET);
    LL_mDelay(150);

    ST7789_Send_Command(ST7789_CMD_SLPOUT);

    // Configure color depth to 12 bit/pixel
    {
        uint8_t Param[] = {0b01010011};
        ST7789_Send_Command_Param(ST7789_CMD_COLMOD, Param, sizeof(Param));
    }

    // Disable mirroring
    {
        uint8_t Param[] = {0x00};
        ST7789_Send_Command_Param(ST7789_CMD_MADCTL, Param, sizeof(Param));
    }

    ST7789_Send_Command(ST7789_CMD_INVON);
    ST7789_Send_Command(ST7789_CMD_NORON);

    ST7789_Init_Memory();

    ST7789_Send_Command(ST7789_CMD_DISPON);

    MX_Display_Set_Window(0, 239, 0, 239);
}

void MX_Display_Init()
{
    Setup_GPIO();
    Setup_SPI();
    Setup_ST7789();
}
