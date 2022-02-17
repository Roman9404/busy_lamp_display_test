#include"shift_register.h"
#include "MBI5153.h"

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
/*
void line_shift_clock (uint16_t delay, uint8_t num)
{
    for (size_t i = 0; i < num; i++)
    {    
        ets_delay_us(delay);
        gpio_set_level(SRCLK,1);
        gpio_set_level(RCLK,0);
        ets_delay_us(delay);
        gpio_set_level(SRCLK,0);
        gpio_set_level(RCLK,1);      
    }
}
*/
/* перемещение "нуля" по разрядам сдвигового регистра - вертикальная развертка*/
/*
void line_shift (uint16_t delay)
{
    gpio_set_level(SER,0);
    line_shift_clock(delay,1);
    ets_delay_us(100);
    gpio_set_level(SER,1);
    line_shift_clock(delay,15);
}
*/
/*настройка таймера*/
void tg0_timer0_init()
{
    timer_config_t config;
    config.alarm_en = 1;
    config.auto_reload = 1;
    config.counter_dir = TIMER_COUNT_UP;
    config.divider = TIMER_DIVIDER;
    config.intr_type = TIMER_INTR_SEL;
    config.counter_en = TIMER_PAUSE;
    /*Configure timer*/
    timer_init(timer_group, timer_idx, &config);
    /*Stop timer counter*/
    timer_pause(timer_group, timer_idx);
    /*Load counter value */
    timer_set_counter_value(timer_group, timer_idx, 0x00000000ULL);// 0x00000000ULL);
    /*Set alarm value*/
    timer_set_alarm_value(timer_group, timer_idx, 5); //количество тиков до прерывания
    /*Enable timer interrupt*/
    timer_enable_intr(timer_group, timer_idx);
    /*Set ISR handler*/
    timer_isr_register(timer_group, timer_idx, timer_group0_isr, (void*) timer_idx, ESP_INTR_FLAG_IRAM, NULL);
    /*Start timer counter*/
    timer_start(timer_group, timer_idx);
}

/*прерывание таймера*/
bool io1 = 1;

int line_count = 0;
int frame_count = 16;

void IRAM_ATTR timer_group0_isr(void *para)
{
    TIMERG0.hw_timer[timer_idx].update = 1;
    TIMERG0.int_clr_timers.t0 = 1;
    TIMERG0.hw_timer[timer_idx].config.alarm_en = 1;
          
    gpio_set_level (MBI_GCLK, io1); //выйдет на GCLK для драйверов 512 тиков и обновление линии.
    io1= !io1;
    line_count++; //счетчик тиков        

    if (line_count==1026) //переход на следующую линию по вертикали
    {
        line_count = 0;
        frame_count--;

        if (frame_count==0)
        {
            gpio_set_level(SER,0);
            frame_count=16;
        }
            
        gpio_set_level(SRCLK,1);
        gpio_set_level(RCLK,0);
        ets_delay_us(5);
        gpio_set_level(SRCLK,0);
        gpio_set_level(RCLK,1);
        gpio_set_level(SER,1);
    }       
}