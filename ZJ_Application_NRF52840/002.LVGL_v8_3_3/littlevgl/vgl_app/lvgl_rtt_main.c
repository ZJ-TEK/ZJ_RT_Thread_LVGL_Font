#include "lvgl_rtt_drv.h" 
#include "lvgl.h" 
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include <rtthread.h> 
#include <rtdevice.h>  
#include "ext_font_app.h"
#include "app_event.h"

#include <math.h>

#include "language.h"


void lv_font_123test_start(FONT_TYPE font)
{
//    lv_obj_t* label = lv_label_create(lv_scr_act());
//    lv_obj_set_style_text_font(label,(lv_font_t *)app_get_font(font),LV_STATE_DEFAULT);
//    lv_label_set_text(label,"Hello ALIENTEK!!!!");

    char* github_addr = "https很牛逼嘛//gitee.com/W23";
    lv_obj_t * label = lv_label_create(lv_scr_act());
    //lv_label_set_recolor(label, true);
    lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR); /*Circular scroll*/
    lv_obj_set_width(label, 240);
    lv_obj_set_style_text_font(label,(lv_font_t *)app_get_font(font),LV_STATE_DEFAULT);
    lv_label_set_text_fmt(label, "#ff0000 Gitee: %s#", github_addr);
    lv_obj_align(label, LV_ALIGN_LEFT_MID, 0, 10);
    
    lv_obj_t * label2 = lv_label_create(lv_scr_act());
    //lv_label_set_recolor(label2, true);
    lv_label_set_long_mode(label2, LV_LABEL_LONG_SCROLL_CIRCULAR); /*Circular scroll*/
    lv_obj_set_width(label2, 240);
    lv_label_set_text_fmt(label2, "#ff0000 Hello# #0000ff world !123456789#");
    lv_obj_align(label2, LV_ALIGN_LEFT_MID, 0, -10);

    NRF_LOG_INFO("lv_font_test_start--->");
    NRF_LOG_FLUSH();
}

void lv_font_test_start(FONT_TYPE font)
{
    static lv_style_t style_bg;
    lv_style_init(&style_bg);
    lv_style_set_radius(&style_bg, 0);
    lv_style_set_text_color(&style_bg, lv_color_hex(0x000000));
    lv_style_set_bg_color(&style_bg, lv_color_hex(0xFFFFFF));
    lv_style_set_bg_opa(&style_bg, LV_OPA_COVER);
    lv_style_set_border_color(&style_bg, lv_color_hex(0x000000));
    lv_style_set_border_opa(&style_bg, LV_OPA_TRANSP);
    lv_style_set_border_width(&style_bg, 0);
    lv_style_set_shadow_width(&style_bg, 0);
    lv_style_set_shadow_spread(&style_bg, 0);
    lv_style_set_shadow_ofs_x(&style_bg, 0);
    lv_style_set_shadow_ofs_y(&style_bg, 0);
    lv_style_set_shadow_color(&style_bg, lv_color_hex(0x000000));
    lv_style_set_pad_all(&style_bg, 0);
    //
    lv_obj_t *page = lv_obj_create(lv_scr_act());
    lv_obj_add_style(page, &style_bg, LV_STATE_DEFAULT);
    lv_obj_set_size(page, 240, 240);
    lv_obj_set_scrollbar_mode(page, LV_SCROLLBAR_MODE_OFF);


    lv_obj_t *label1 = lv_label_create(page);
//    lv_label_set_long_mode(label1, LV_LABEL_LONG_SCROLL_CIRCULAR); /*Circular scroll*/
//    lv_obj_set_style_text_color(label1, lv_color_hex(0xff00ff), LV_PART_MAIN);
    lv_obj_set_style_text_font(label1, app_get_font(APP_INNER_FONT_SIZE_24), LV_PART_MAIN);
    lv_label_set_text_fmt(label1, "Innerfont");
    lv_obj_set_size(label1, 200, 40);
    lv_obj_align(label1, LV_ALIGN_TOP_LEFT, 30, 10);
    lv_obj_set_style_text_align(label1, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN);

    lv_obj_t *label2 = lv_label_create(page);
    lv_label_set_long_mode(label2, LV_LABEL_LONG_SCROLL_CIRCULAR); /*Circular scroll*/
//    lv_obj_set_style_text_color(label2, lv_color_hex(0xff00ff), LV_PART_MAIN);
    lv_obj_set_style_text_font(label2, app_get_font(APP_FONT_SIZE_24), LV_PART_MAIN);
    lv_label_set_text_fmt(label2, APP_GET_STR(key_setting, "setting"));
    lv_obj_set_size(label2, 200, 40);
    lv_obj_align(label2, LV_ALIGN_TOP_LEFT, 30, 40);
    lv_obj_set_style_text_align(label2, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN);

    lv_obj_t * label3 = lv_label_create(page);
//    lv_label_set_long_mode(label3, LV_LABEL_LONG_SCROLL_CIRCULAR); /*Circular scroll*/
//    lv_obj_set_style_text_color(label3, lv_color_hex(0xff00ff), LV_PART_MAIN);
    lv_obj_set_style_text_font(label3, app_get_font(APP_ICON_FONT_SIZE_32), LV_PART_MAIN);
    lv_label_set_text_fmt(label3, "3"ICON_SYMBOL_RIGHT ICON_SYMBOL_KEYBOARD ICON_SYMBOL_SAVE);
    lv_obj_set_size(label3, 200, 40);
    lv_obj_align(label3, LV_ALIGN_TOP_LEFT, 30, 80);
    lv_obj_set_style_text_align(label3, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN);

    lv_obj_t * label4 = lv_label_create(page);
//    lv_label_set_long_mode(label4, LV_LABEL_LONG_SCROLL_CIRCULAR); /*Circular scroll*/
//    lv_obj_set_style_text_color(label4, lv_color_hex(0xff00ff), LV_PART_MAIN);
    lv_obj_set_style_text_font(label4, app_get_font(APP_EMOJI_FONT_SIZE_32), LV_PART_MAIN);
    lv_label_set_text_fmt(label4, "4" EMOJI_SYMBOL_0x1F604 EMOJI_SYMBOL_0x1F605 EMOJI_SYMBOL_0x1F606 EMOJI_SYMBOL_0x1F607);
    lv_obj_set_size(label4, 200, 40);
    lv_obj_align(label4, LV_ALIGN_TOP_LEFT, 30, 120);
    lv_obj_set_style_text_align(label4, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN);

    lv_obj_t * label5 = lv_label_create(page);
//    lv_label_set_long_mode(label5, LV_LABEL_LONG_SCROLL_CIRCULAR); /*Circular scroll*/
//    lv_obj_set_style_text_color(label5, lv_color_hex(0xff00ff), LV_PART_MAIN);
    lv_obj_set_style_text_font(label5, app_get_font(APP_FULL_FONT_SIZE_24), LV_PART_MAIN);
    lv_label_set_text_fmt(label5, APP_GET_STR(key_alarm, "alam"));
    lv_obj_set_size(label5, 200, 40);
    lv_obj_align(label5, LV_ALIGN_TOP_LEFT, 30, 160);
    lv_obj_set_style_text_align(label5, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN);

    lv_obj_t * label6 = lv_label_create(page);
//    lv_label_set_long_mode(label6, LV_LABEL_LONG_SCROLL_CIRCULAR); /*Circular scroll*/
//    lv_obj_set_style_text_color(label6, lv_color_hex(0xff00ff), LV_PART_MAIN);
    lv_obj_set_style_text_font(label6, app_get_font(APP_FULL_FONT_SIZE_24), LV_PART_MAIN);
    lv_label_set_text_fmt(label6, "Rtthread"ICON_SYMBOL_GPS2 EMOJI_SYMBOL_0x200D EMOJI_SYMBOL_0x1F601);
    lv_obj_set_size(label6, 200, 40);
    lv_obj_align(label6, LV_ALIGN_TOP_LEFT, 30, 200);
    lv_obj_set_style_text_align(label6, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN);


    NRF_LOG_INFO("lv_font_test_start--->");
    NRF_LOG_FLUSH();
}


static void lvgl_demo_run(void *p)
{
    
    lv_font_test_start(APP_FULL_FONT_SIZE_24);
    while(1)
    {
        rt_thread_delay(RT_TICK_PER_SECOND/100);
        lv_task_handler();
//        cnt++;
//        if(cnt >= 100)
//        {
//            cnt = 0;
//            lightmeter_lux_read();
//        }
    }
} 

int rt_lvgl_demo_init(void)
{
    rt_err_t ret = RT_EOK;
    rt_thread_t thread = RT_NULL; 
    app_font_init();
    /* init littlevGL */ 
    ret = lvgl_rtt_init("zj_lcd"); 
    if(ret != RT_EOK)
    {
        return ret; 
    }

    /* littleGL demo gui thread */ 
    thread = rt_thread_create("lvgl_rtt", lvgl_demo_run, RT_NULL, 10*1024, 5, 10); 
    if(thread == RT_NULL)
    {
        return RT_ERROR;
    }
    rt_thread_startup(thread);

    return RT_EOK; 
}
INIT_APP_EXPORT(rt_lvgl_demo_init); 
