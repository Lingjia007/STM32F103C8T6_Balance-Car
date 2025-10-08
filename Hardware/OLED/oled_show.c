#include "oled_show.h"

void OLED_Show(void)
{
    //=============第一行显示小车模式=======================//
    if (Mode == 0 || Mode == 1 || Mode == 2)
    {
        if (Way_Angle == 1)
            OLED_HAL_ShowString(0, 0, (uint8_t *)"DMP", 12, 1);
        else if (Way_Angle == 2)
            OLED_HAL_ShowString(0, 0, (uint8_t *)"Kalman", 12, 1);
        else if (Way_Angle == 3)
            OLED_HAL_ShowString(0, 0, (uint8_t *)"C F", 12, 1);
    }
    if (Mode == 3 || Mode == 4 || Mode == 5)
    {
        if (LD_Successful_Receive_flag == 1)
            OLED_HAL_ShowString(0, 0, (uint8_t *)"SUCCES", 12, 1);
        else
            OLED_HAL_ShowString(0, 0, (uint8_t *)"DEFEAT", 12, 1);
    }

    if (Mode == 2)
        OLED_HAL_ShowString(60, 0, (uint8_t *)"U_Follow", 12, 1);
    else if (Mode == 1)
        OLED_HAL_ShowString(60, 0, (uint8_t *)"U_Avoid ", 12, 1);
    else if (Mode == 0)
        OLED_HAL_ShowString(60, 0, (uint8_t *)"Normal  ", 12, 1);
    else if (Mode == 3)
        OLED_HAL_ShowString(60, 0, (uint8_t *)"LD_Avoid", 12, 1);
    else if (Mode == 4)
        OLED_HAL_ShowString(60, 0, (uint8_t *)"LD_Follow", 12, 1);
    else if (Mode == 5)
        OLED_HAL_ShowString(60, 0, (uint8_t *)"LD_Straight", 12, 1);
    else if (Mode == 6)
    {
        // OLED_Show_CCD(); // 需要实现CCD显示函数
    }
    else if (Mode == 7)
    {
        // OLED_HAL_ShowNumber(0, 0, Sensor_Left, 5, 12);
        // OLED_HAL_ShowNumber(30, 0, Sensor_Middle, 4, 12);
        // OLED_HAL_ShowNumber(60, 0, Sensor_Right, 4, 12);
        // OLED_HAL_ShowNumber(90, 0, Sensor, 4, 12);
    }

    //=============第二行显示角度=======================//
    OLED_HAL_ShowString(0, 10, (uint8_t *)"Angle", 12, 1);
    // 先清空角度显示区域（从48像素开始到屏幕结束）
    OLED_HAL_ShowString(48, 10, (uint8_t *)"        ", 12, 1);
    OLED_HAL_ShowFloat(48, 10, Angle_Balance, 3, 12);

    //=============第三行显示角速度与距离===============//
    OLED_HAL_ShowString(0, 20, (uint8_t *)"Gyrox", 12, 1);
    if (Gyro_Balance < 0)
        OLED_HAL_ShowString(42, 20, (uint8_t *)"-", 12, 1);
    if (Gyro_Balance >= 0)
        OLED_HAL_ShowString(42, 20, (uint8_t *)"+", 12, 1);
    OLED_HAL_ShowNumber(50, 20, myabs((int)Gyro_Balance), 4, 12);

    OLED_HAL_ShowNumber(82, 20, (u16)Distance, 5, 12);
    OLED_HAL_ShowString(114, 20, (uint8_t *)"mm", 12, 1);

    //=============第四行显示左编码器PWM与读数=======================//
    OLED_HAL_ShowString(0, 30, (uint8_t *)"L", 12, 1);
    if (Motor_Left < 0)
    {
        OLED_HAL_ShowString(16, 30, (uint8_t *)"-", 12, 1);
        OLED_HAL_ShowNumber(26, 30, myabs((int)Motor_Left), 4, 12);
    }
    if (Motor_Left >= 0)
    {
        OLED_HAL_ShowString(16, 30, (uint8_t *)"+", 12, 1);
        OLED_HAL_ShowNumber(26, 30, myabs((int)Motor_Left), 4, 12);
    }

    if (Velocity_Left < 0)
        OLED_HAL_ShowString(60, 30, (uint8_t *)"-", 12, 1);
    if (Velocity_Left >= 0)
        OLED_HAL_ShowString(60, 30, (uint8_t *)"+", 12, 1);
    OLED_HAL_ShowNumber(68, 30, myabs((int)Velocity_Left), 4, 12);
    OLED_HAL_ShowString(96, 30, (uint8_t *)"mm/s", 12, 1);

    //=============第五行显示右编码器PWM与读数=======================//
    OLED_HAL_ShowString(0, 40, (uint8_t *)"R", 12, 1);
    if (Motor_Right < 0)
    {
        OLED_HAL_ShowString(16, 40, (uint8_t *)"-", 12, 1);
        OLED_HAL_ShowNumber(26, 40, myabs((int)Motor_Right), 4, 12);
    }
    if (Motor_Right >= 0)
    {
        OLED_HAL_ShowString(16, 40, (uint8_t *)"+", 12, 1);
        OLED_HAL_ShowNumber(26, 40, myabs((int)Motor_Right), 4, 12);
    }

    if (Velocity_Right < 0)
        OLED_HAL_ShowString(60, 40, (uint8_t *)"-", 12, 1);
    if (Velocity_Right >= 0)
        OLED_HAL_ShowString(60, 40, (uint8_t *)"+", 12, 1);
    OLED_HAL_ShowNumber(68, 40, myabs((int)Velocity_Right), 4, 12);
    OLED_HAL_ShowString(96, 40, (uint8_t *)"mm/s", 12, 1);

    //=============第六行显示电压与电机开关=======================//
    OLED_HAL_ShowString(0, 50, (uint8_t *)"V", 12, 1);
    OLED_HAL_ShowString(30, 50, (uint8_t *)".", 12, 1);
    OLED_HAL_ShowString(64, 50, (uint8_t *)"V", 12, 1);
    OLED_HAL_ShowNumber(19, 50, (u16)Voltage / 100, 2, 12);
    OLED_HAL_ShowNumber(42, 50, (u16)Voltage / 10 % 10, 1, 12);
    OLED_HAL_ShowNumber(50, 50, (u16)Voltage % 10, 1, 12);

    if (Flag_Stop)
        OLED_HAL_ShowString(95, 50, (uint8_t *)"OFF", 12, 1);
    if (!Flag_Stop)
        OLED_HAL_ShowString(95, 50, (uint8_t *)"ON ", 12, 1);

    //=============刷新=======================//
    OLED_HAL_Refresh();
}
