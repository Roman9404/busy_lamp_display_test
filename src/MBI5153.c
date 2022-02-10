#include"MBI5153.h"

extern bool latch;
extern bool reg1;
extern uint16_t config_reg;

uint16_t line_1 [16] = {red, red, black, black, black, black, red, red, black, red, black, red, black, red, black, red};

/*инициализация gpio для MBI5153*/
void MBI_gpio_init()
{
    //gpio_pad_select_gpio(MBI_GCLK);
    gpio_pad_select_gpio(MBI_LE);
    gpio_pad_select_gpio(MBI_DCLK);
    gpio_pad_select_gpio(MBI_SDI);

    //gpio_set_direction(MBI_GCLK, GPIO_MODE_OUTPUT);
    gpio_set_direction(MBI_LE, GPIO_MODE_OUTPUT);
    gpio_set_direction(MBI_DCLK, GPIO_MODE_OUTPUT);
    gpio_set_direction(MBI_SDI, GPIO_MODE_OUTPUT);

    gpio_set_level(MBI_LE,0);
    gpio_set_level(MBI_DCLK,0);
}

/* тактирование линии данных драйвера MBI5153*/
void mbi_clock (uint8_t delay,uint8_t clock)
{
    while (clock--)
    {
        vTaskDelay(delay / portTICK_PERIOD_MS);
        gpio_set_level(MBI_DCLK,1);
        vTaskDelay(delay / portTICK_PERIOD_MS);
        gpio_set_level(MBI_DCLK,0);
        vTaskDelay(delay / portTICK_PERIOD_MS);
    }
}

/*настройка регистра конфигурации*/
void mbi_configuration(uint8_t ghost_elimination, uint8_t line_num, uint8_t gray_scale, uint8_t gclk_multiplier,uint8_t current)
{
    config_reg = 0;
    config_reg = (config_reg | (ghost_elimination<<15) | (line_num<<8) | (gray_scale<<7) | (gclk_multiplier<<6) | (current));
    mbi_set_config(config_reg);
}

/*отправка регистра конфигурации*/
void mbi_set_config (uint16_t config)
{
    bool config_bit[16] = {0};
    int data_mask = 0x1;
    //printf("test data = %x \n", config);
    for (size_t i = 0; i < 16; i++)  //парсит байт в булевый массив побитно
    {
        config_bit[i] = config & (data_mask<<i);
        //printf("data mask = %d config bit[%d] = %d \n",(data_mask<<i),i,config_bit[i]);
    }
    
    for (size_t i = 16; i > 0; i--)  //отсылает данные на ногу контроллера из булевого массива
    {
        if ((i<5) && (reg1==1)) //защелка данных по последним 4 битам, при отправке последнего байта присвоить reg1=1
        {
            gpio_set_level(MBI_LE,1);
            gpio_set_level(MBI_SDI,config_bit[i-1]);
            mbi_clock(5,1);
            //gpio_set_level(MBI_SDI,0);            
        }
        else
        {
            gpio_set_level(MBI_SDI,config_bit[i-1]);
            mbi_clock(5,1);
            //gpio_set_level(MBI_SDI,0);
        } 
    }
    gpio_set_level(MBI_LE,0);
    gpio_set_level(MBI_SDI,0);
    reg1=0;
}

/*передача 2 байт данных драйверу MBI5153*/
void mbi_set_data (uint16_t data)
{
    bool data_bit[16] = {0};
    int data_mask = 0x1;
    printf("test data = %d \n", data);
    for (size_t i = 0; i < 16; i++)  //парсит байт в булевый массив побитно
    {
        data_bit[i] = data & (data_mask<<i);
       // printf("data mask = %d data bit[%d] = %d \n",(data_mask<<i),i,data_bit[i]);
    }
    for (size_t i = 16; i > 0; i--)  //отсылает данные на ногу контроллера из булевого массива
    {
        if ((i==1) && (latch==1)) //защелка данных по последнему биту, при отправке последнего байта присвоить latch=1
        {
            gpio_set_level(MBI_LE,1);
            gpio_set_level(MBI_SDI,data_bit[i-1]);
            mbi_clock(5,1);
            gpio_set_level(MBI_SDI,0);
            gpio_set_level(MBI_LE,0);
            latch = 0;
        }
        else
        {
            gpio_set_level(MBI_SDI,data_bit[i-1]);
            mbi_clock(5,1);
            gpio_set_level(MBI_SDI,0);
        } 
    }
    gpio_set_level(MBI_SDI,0);
}

/*команда предактивации - отправляется перед отправкой данных регистра конфигурации*/
void PreActive ()
{
    gpio_set_level(MBI_LE,1);
    mbi_clock(5,14);
    gpio_set_level(MBI_LE,0);
}

/*вертикальная синхронизация - обновляет данные кадра на выходах, использовать вместе с вертикальной разверткой*/
void VertSync ()
{
    gpio_set_level(MBI_LE,1);
    mbi_clock(5,2);
    gpio_set_level(MBI_LE,0);
}

/*программный сброс*/
void soft_reset()
{
    gpio_set_level(MBI_LE,1);
    mbi_clock(5,10);
    gpio_set_level(MBI_LE,0); 
}

/*отсылает данные для 1 линии строчной развертки*/
void mbi_set_frame()
{
    for (size_t i = 0; i < 16; i++)
    {
        mbi_set_data(line_1[i]);
        mbi_set_data(line_1[i]);
        latch=1;
        mbi_set_data(line_1[i]);
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    mbi_clock(5,50);
    VertSync();
}