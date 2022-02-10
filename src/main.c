#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#include "shift_register.h"
#include "MBI5153.h"


#define BLINK_GPIO              2   //светодиод

bool latch = 0;
bool reg1 = 0;
uint16_t config_reg = 0;

void app_main()
{
    gpio_pad_select_gpio(BLINK_GPIO);

    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

    shift_reg_gpio_init ();
    MBI_gpio_init();

    while(1) 
    {
       //line_shift(50); //вертикальная развертка
       PreActive();
       vTaskDelay(50 / portTICK_PERIOD_MS);
       mbi_configuration(ghost_elimination_ON,line_num_16,gray_scale_14,gclk_multiplier_ON,current_1);
       mbi_configuration(ghost_elimination_ON,line_num_16,gray_scale_14,gclk_multiplier_ON,current_1);
       reg1=1;
       mbi_configuration(ghost_elimination_ON,line_num_16,gray_scale_14,gclk_multiplier_ON,current_1);
       //mbi_set_config();
       gpio_set_level(BLINK_GPIO,1);
       vTaskDelay(3000 / portTICK_PERIOD_MS);
       gpio_set_level(BLINK_GPIO,0);
    }

}