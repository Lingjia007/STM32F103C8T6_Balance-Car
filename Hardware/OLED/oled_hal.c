#include "OLED_HAL.h"
#include "oledfont.h"

u8 OLED_GRAM[144][8];

// 向OLED写入一个字节
void OLED_HAL_WR_Byte(u8 dat, u8 mode)
{
    if (mode)
        OLED_DC_Set();
    else
        OLED_DC_Clr();

    OLED_CS_Clr();

    // 使用HAL库的SPI发送函数
    HAL_SPI_Transmit(&hspi1, &dat, 1, 100);

    OLED_CS_Set();
    OLED_DC_Set();
}

// 开启OLED显示
void OLED_HAL_DisPlay_On(void)
{
    OLED_HAL_WR_Byte(0x8D, OLED_CMD); // 电荷泵使能
    OLED_HAL_WR_Byte(0x14, OLED_CMD); // 开启电荷泵
    OLED_HAL_WR_Byte(0xAF, OLED_CMD); // 开启OLED
}

// 关闭OLED显示
void OLED_HAL_DisPlay_Off(void)
{
    OLED_HAL_WR_Byte(0x8D, OLED_CMD); // 电荷泵使能
    OLED_HAL_WR_Byte(0x10, OLED_CMD); // 关闭电荷泵
    OLED_HAL_WR_Byte(0xAE, OLED_CMD); // 关闭OLED
}

// 更新显存到OLED
void OLED_HAL_Refresh(void)
{
    u8 i, n;
    for (i = 0; i < 8; i++)
    {
        OLED_HAL_WR_Byte(0xb0 + i, OLED_CMD); // 设置页地址
        OLED_HAL_WR_Byte(0x00, OLED_CMD);     // 设置列低地址
        OLED_HAL_WR_Byte(0x10, OLED_CMD);     // 设置列高地址
        for (n = 0; n < 128; n++)
            OLED_HAL_WR_Byte(OLED_GRAM[n][i], OLED_DATA);
    }
}

// 清屏函数
void OLED_HAL_Clear(void)
{
    u8 i, n;
    for (i = 0; i < 8; i++)
    {
        for (n = 0; n < 128; n++)
        {
            OLED_GRAM[n][i] = 0; // 清除所有数据
        }
    }
    OLED_HAL_Refresh(); // 更新显示
}

// 画点函数
// x:0~127
// y:0~63
// t:1 显示 0,不显示
void OLED_HAL_DrawPoint(u8 x, u8 y, u8 t)
{
    u8 i, m, n;
    i = y / 8;
    m = y % 8;
    n = 1 << m;
    if (t)
    {
        OLED_GRAM[x][i] |= n;
    }
    else
    {
        OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
        OLED_GRAM[x][i] |= n;
        OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
    }
}

// 在指定位置显示一个字符
// x:0~127
// y:0~63
// size1:选择字体 6x8/6x12/8x16/12x24
// mode:0,反白显示;1,正常显示
void OLED_HAL_ShowChar(u8 x, u8 y, u8 chr, u8 size1, u8 mode)
{
    u8 i, m, temp, size2, chr1;
    u8 x0 = x, y0 = y;
    if (size1 == 8)
        size2 = 6;
    else
        size2 = (size1 / 8 + ((size1 % 8) ? 1 : 0)) * (size1 / 2);
    chr1 = chr - ' '; // 得到偏移后的值
    for (i = 0; i < size2; i++)
    {
        if (size1 == 8)
            temp = asc2_0806[chr1][i];
        else if (size1 == 12)
            temp = asc2_1206[chr1][i];
        else if (size1 == 16)
            temp = asc2_1608[chr1][i];
        else if (size1 == 24)
            temp = asc2_2412[chr1][i];
        else
            return;

        for (m = 0; m < 8; m++)
        {
            if (temp & 0x01)
                OLED_HAL_DrawPoint(x, y, mode);
            else
                OLED_HAL_DrawPoint(x, y, !mode);
            temp >>= 1;
            y++;
        }

        x++;
        if ((size1 != 8) && ((x - x0) == size1 / 2))
        {
            x = x0;
            y0 = y0 + 8;
        }
        y = y0;
    }
}

// 显示字符串
// x,y:起始坐标
// size1:字体大小
//*chr:字符串起始地址
// mode:0,反白显示;1,正常显示
void OLED_HAL_ShowString(u8 x, u8 y, u8 *chr, u8 size1, u8 mode)
{
    while ((*chr >= ' ') && (*chr <= '~'))
    {
        OLED_HAL_ShowChar(x, y, *chr, size1, mode);
        if (size1 == 8)
            x += 6;
        else
            x += size1 / 2;
        chr++;
    }
}

// 计算10的幂次方
static u32 OLED_Pow(u8 m, u8 n)
{
    u32 result = 1;
    while (n--)
        result *= m;
    return result;
}

// 显示数字
// x,y:起始坐标
// num:要显示的数字
// len:数字的位数
// size1:字体大小
void OLED_HAL_ShowNumber(u8 x, u8 y, u32 num, u8 len, u8 size1)
{
    u8 t, temp;
    u8 enshow = 0;
    
    for (t = 0; t < len; t++)
    {
        temp = (num / OLED_Pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                OLED_HAL_ShowChar(x + (size1 / 2) * t, y, ' ', size1, 1);
                continue;
            }
            else
                enshow = 1;
        }
        OLED_HAL_ShowChar(x + (size1 / 2) * t, y, temp + '0', size1, 1);
    }
}

// 显示浮点数（两位小数）
// x,y:起始坐标
// num:要显示的浮点数
// int_len:整数部分的位数
// size1:字体大小
void OLED_HAL_ShowFloat(u8 x, u8 y, float num, u8 int_len, u8 size1)
{
    int integer_part;
    int decimal_part;
    
    // 处理负数
    if (num < 0)
    {
        OLED_HAL_ShowChar(x, y, '-', size1, 1);
        num = -num;
        x += size1 / 2; // 移动位置给负号留空间
    }
    
    // 提取整数部分和小数部分
    integer_part = (int)num;
    decimal_part = (int)((num - integer_part) * 100 + 0.5); // 四舍五入到两位小数
    
    // 显示整数部分
    OLED_HAL_ShowNumber(x, y, integer_part, int_len, size1);
    
    // 显示小数点
    OLED_HAL_ShowChar(x + (size1 / 2) * int_len, y, '.', size1, 1);
    
    // 显示小数部分（两位）
    OLED_HAL_ShowNumber(x + (size1 / 2) * (int_len + 1), y, decimal_part, 2, size1);
}

// OLED初始化
void OLED_HAL_Init(void)
{
    // 初始化已经在CubeMX生成的代码中完成
    // 这里只需要处理OLED的复位和初始化命令

    OLED_RES_Clr();
    HAL_Delay(200);
    OLED_RES_Set();

    OLED_HAL_WR_Byte(0xAE, OLED_CMD); // 关闭OLED面板
    OLED_HAL_WR_Byte(0x00, OLED_CMD); // 设置低列地址
    OLED_HAL_WR_Byte(0x10, OLED_CMD); // 设置高列地址
    OLED_HAL_WR_Byte(0x40, OLED_CMD); // 设置起始行地址
    OLED_HAL_WR_Byte(0x81, OLED_CMD); // 设置对比度控制寄存器
    OLED_HAL_WR_Byte(0xCF, OLED_CMD); // 设置亮度
    OLED_HAL_WR_Byte(0xA1, OLED_CMD); // 设置段/列映射
    OLED_HAL_WR_Byte(0xC8, OLED_CMD); // 设置COM/行扫描方向
    OLED_HAL_WR_Byte(0xA6, OLED_CMD); // 设置正常显示
    OLED_HAL_WR_Byte(0xA8, OLED_CMD); // 设置多路复用比
    OLED_HAL_WR_Byte(0x3f, OLED_CMD); // 1/64占空比
    OLED_HAL_WR_Byte(0xD3, OLED_CMD); // 设置显示偏移
    OLED_HAL_WR_Byte(0x00, OLED_CMD); // 不偏移
    OLED_HAL_WR_Byte(0xd5, OLED_CMD); // 设置显示时钟分频比/振荡器频率
    OLED_HAL_WR_Byte(0x80, OLED_CMD); // 设置分频比
    OLED_HAL_WR_Byte(0xD9, OLED_CMD); // 设置预充电周期
    OLED_HAL_WR_Byte(0xF1, OLED_CMD); // 设置预充电
    OLED_HAL_WR_Byte(0xDA, OLED_CMD); // 设置COM引脚硬件配置
    OLED_HAL_WR_Byte(0x12, OLED_CMD);
    OLED_HAL_WR_Byte(0xDB, OLED_CMD); // 设置VCOMH
    OLED_HAL_WR_Byte(0x40, OLED_CMD); // 设置VCOM取消选择级别
    OLED_HAL_WR_Byte(0x20, OLED_CMD); // 设置页寻址模式
    OLED_HAL_WR_Byte(0x02, OLED_CMD);
    OLED_HAL_WR_Byte(0x8D, OLED_CMD); // 设置电荷泵启用/禁用
    OLED_HAL_WR_Byte(0x14, OLED_CMD); // 启用
    OLED_HAL_WR_Byte(0xA4, OLED_CMD); // 禁用整个显示
    OLED_HAL_WR_Byte(0xA6, OLED_CMD); // 禁用反向显示

    OLED_HAL_Clear();
    OLED_HAL_WR_Byte(0xAF, OLED_CMD); // 开启显示
}
