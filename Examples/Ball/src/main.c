#include <MX_System.h>
#include <MX_Display.h>

const int RectWidth = 20;

int RectX = 240 / 2 - RectWidth / 2;
int RectY = 240 / 2 - RectWidth / 2;

int CoefX = 1;
int CoefY = 2;

void UpdateRectPosition()
{
    if (RectX >= 239 - RectWidth)
    {
        CoefX = -CoefX;
    }
    if (RectX <= 0)
    {
        CoefX = -CoefX;
    }
    if (RectY >= 239 - RectWidth)
    {
        CoefY = -CoefY;
    }
    if (RectY <= 0)
    {
        CoefY = -CoefY;
    }

    RectX += CoefX;
    RectY += CoefY;
}

int IsPixelInsideRect(int x, int y)
{
    if ((x >= RectX) && (x < RectX + RectWidth))
    {
        if ((y >= RectY) && (y < RectY + RectWidth))
        {
            return 1;
        }
    }

    return 0;
}

int main()
{
    MX_System_Init();
    MX_Display_Init();

    while (1)
    {
        UpdateRectPosition();

        for (int i = 0; i < 240; i++)
        {
            for (int j = 0; j < 240; j++)
            {
                if (IsPixelInsideRect(j, i))
                {
                    MX_Display_Send_Pixel(0xF00);
                }
                else
                {
                    MX_Display_Send_Pixel(0x000);
                }
            }
        }
    }

    return 0;
}
