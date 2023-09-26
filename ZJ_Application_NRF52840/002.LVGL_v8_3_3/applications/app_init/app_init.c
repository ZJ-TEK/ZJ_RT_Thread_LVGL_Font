#include <stdint.h>
#include <string.h>
#include <rtthread.h>
#include "nrf_log.h"
#include "board.h"
#include "app_init.h"
#include "app_uart.h"
#include "app_gpio.h"
#include "app_event.h"

#include "bsp_lcd_ie3130b_dri.h"
#include "bsp_qspi_dri.h"
#include "ext_font_app.h"
#include "ext_font_engine.h"

#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include <math.h>
#define font_printf(...) 	do{NRF_LOG_INFO(__VA_ARGS__);/*rt_kprintf(__VA_ARGS__);*/}while(0)    

int main(void)
{
    APP_EVENT_PUSH_DELAY(led_event_start,NULL,500);
    rt_kprintf("RT-Thread for nrf52840 started");
    return RT_TRUE;
}


/**
 * @}
 */
