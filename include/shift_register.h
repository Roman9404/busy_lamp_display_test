#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#define SER                     15   //данные, которые будут сдвигаться в регистрах
#define OE                      4    //outpu enable
#define RCLK                    18   //такт защелки
#define SRCLR                   19   //очитска регистра
#define SRCLK                   21   //тактовый сигнал

void shift_reg_gpio_init ();
void line_shift_clock (uint16_t delay, uint8_t num);
void line_shift (uint16_t delay);