#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#define MBI_GCLK                23  //тактирование уровней серого
#define MBI_LE                  12  //данные/команда
#define MBI_DCLK                14  //тактирование линии данных
#define MBI_SDI                 27  //данные

#define ghost_elimination_ON    1   //послесвечение выключенно
#define ghost_elimination_OFF   0   //послесвечение включено
#define gray_scale_13           1   //шкала серого 13 бит
#define gray_scale_14           0   //шкала серого 14 бит
#define gclk_multiplier_ON      1   //делитель gclk включен
#define gclk_multiplier_OFF     0   //делитель gclk выключен
#define line_num_16             15  //количество линий 16
#define line_num_1              0   //количество линий 1
#define current_1               15  //ток на светодиоде

#define l                   65532  
#define o                   0 


void MBI_gpio_init();
void mbi_clock (uint8_t delay,uint8_t clock);
void mbi_GCLK_clock (uint32_t GCLK_clock);
void mbi_configuration(uint8_t ghost_elimination, uint8_t line_num, uint8_t gray_scale, uint8_t gclk_multiplier,uint8_t current);
void mbi_set_config (uint16_t config);
void mbi_set_data (uint16_t data);
void PreActive ();
void VertSync ();
void soft_reset();
void mbi_set_frame();