#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#include "shift_register.h"
#include "MBI5153.h"

#include "driver/ledc.h"
#include "esp_err.h"

#define BLINK_GPIO              2   //светодиод

#define LEDC_TIMER              LEDC_TIMER_1
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_OUTPUT_IO          (23) // Define the output GPIO
#define LEDC_CHANNEL            LEDC_CHANNEL_1
#define LEDC_DUTY_RES           LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define LEDC_DUTY               (4095) // Set duty to 50%. ((2 ** 13) - 1) * 50% = 4095
#define LEDC_FREQUENCY          (5120) // Frequency in Hertz. Set frequency at 5 kHz

static void example_ledc_init(void)
{
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE,
        .timer_num        = LEDC_TIMER,
        .duty_resolution  = LEDC_DUTY_RES,
        .freq_hz          = LEDC_FREQUENCY,  // Set output frequency at 5 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CHANNEL,
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = LEDC_OUTPUT_IO,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}


bool latch = 0;
bool reg1 = 0;
uint16_t config_reg = 0;

void app_main()
{
    gpio_pad_select_gpio(BLINK_GPIO);

    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

//    example_ledc_init();
    // Set duty to 50%
//    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, LEDC_DUTY));
    // Update duty to apply the new value
//    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));

    //tg0_timer0_init();

    shift_reg_gpio_init ();
    MBI_gpio_init();

    soft_reset();
    PreActive();
    mbi_configuration(ghost_elimination_OFF,line_num_16,gray_scale_14,gclk_multiplier_OFF,current_1);
    mbi_configuration(ghost_elimination_OFF,line_num_16,gray_scale_14,gclk_multiplier_OFF,current_1);
    reg1=1;
    mbi_configuration(ghost_elimination_OFF,line_num_16,gray_scale_14,gclk_multiplier_OFF,current_1);
    mbi_set_frame();
    VertSync();
    VertSync();

    tg0_timer0_init();      

    while(1) 
    {
       //line_shift(100000); //вертикальная развертка
    }

}