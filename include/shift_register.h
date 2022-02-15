#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#include "freertos/queue.h"
#include "soc/timer_group_struct.h"
#include "driver/periph_ctrl.h"
#include "driver/timer.h"
#include "esp_types.h"

#define SER                     15   //данные, которые будут сдвигаться в регистрах
#define OE                      4    //outpu enable
#define RCLK                    18   //такт защелки
#define SRCLR                   19   //очитска регистра
#define SRCLK                   21   //тактовый сигнал

#define timer_test_1            33
#define timer_test_2            32

#define TIMER_INTR_SEL TIMER_INTR_LEVEL  /*!< Timer level interrupt */
#define TIMER_GROUP    TIMER_GROUP_0     /*!< Test on timer group 1 */
#define TIMER_DIVIDER   10               /*!< Hardware timer clock divider, 80 to get 1MHz clock to timer предделитель */
#define TIMER_SCALE    (TIMER_BASE_CLK / TIMER_DIVIDER)  /*!< used to calculate counter value */
#define TIMER_FINE_ADJ   (0*(TIMER_BASE_CLK / TIMER_DIVIDER)/1000000) /*!< used to compensate alarm value */
#define TIMER_INTERVAL0_SEC   (0.01)   /*!< test interval for timer 0 */

void shift_reg_gpio_init ();
void line_shift_clock (uint16_t delay, uint8_t num);
void line_shift (uint16_t delay);

void tg0_timer0_init();
void IRAM_ATTR timer_group0_isr(void *para);