#include"shift_register.h"

void shift_reg_gpio_init ()
{
    gpio_pad_select_gpio(SER);
    gpio_pad_select_gpio(OE);
    gpio_pad_select_gpio(RCLK);
    gpio_pad_select_gpio(SRCLR);
    gpio_pad_select_gpio(SRCLK);

    gpio_set_direction(SER, GPIO_MODE_OUTPUT);
    gpio_set_direction(OE, GPIO_MODE_OUTPUT);
    gpio_set_direction(RCLK, GPIO_MODE_OUTPUT);
    gpio_set_direction(SRCLR, GPIO_MODE_OUTPUT);
    gpio_set_direction(SRCLK, GPIO_MODE_OUTPUT);
}
/* тактирование сдвиговых регистров*/
void line_shift_clock (uint16_t delay, uint8_t num)
{
    for (size_t i = 0; i < num; i++)
    {    
        vTaskDelay(delay / portTICK_PERIOD_MS);
        gpio_set_level(SRCLK,1);
        gpio_set_level(RCLK,0);
        vTaskDelay(delay / portTICK_PERIOD_MS);
        gpio_set_level(SRCLK,0);
        gpio_set_level(RCLK,1);
    }
}

/* перемещение "единицы" по разрядам сдвигового регистра - вертикальная развертка*/
void line_shift (uint16_t delay)
{
    gpio_set_level(SER,0);
    line_shift_clock(delay,1);
    vTaskDelay(delay / portTICK_PERIOD_MS);
    gpio_set_level(SER,1);
    line_shift_clock(delay,15);
}