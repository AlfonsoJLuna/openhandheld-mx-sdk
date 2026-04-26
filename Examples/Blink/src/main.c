#include <MX_System.h>
#include <MX_LED.h>

int main()
{
    MX_System_Init();
    MX_LED_Init();

    while (1)
    {
        MX_LED_On(1, 0, 0);
        MX_System_Delay(1000);

        MX_LED_On(0, 1, 0);
        MX_System_Delay(1000);
        
        MX_LED_On(0, 0, 1);
        MX_System_Delay(1000);  
    }

    return 0;
}
