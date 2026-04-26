#include <MX_System.h>
#include <MX_LED.h>
#include <MX_Button.h>

int main()
{
    MX_System_Init();
    MX_LED_Init();
    MX_Button_Init();

    while (1)
    {
        if (MX_Button_SELECT())
        {
            MX_LED_On(1, 0, 0);
        }

        if (MX_Button_START())
        {
            MX_LED_On(0, 1, 0);
        }

        if (MX_Button_B())
        {
            MX_LED_On(0, 0, 1);
        }

        if (MX_Button_A())
        {
            MX_LED_On(1, 1, 1);
        }

        MX_System_Delay(100);  
    }

    return 0;
}
