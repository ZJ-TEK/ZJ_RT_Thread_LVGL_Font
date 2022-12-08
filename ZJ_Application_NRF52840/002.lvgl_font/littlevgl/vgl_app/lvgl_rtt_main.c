#include "lvgl_rtt_drv.h" 
#include "lvgl.h" 

#include <rtthread.h> 
#include <rtdevice.h>  
#include "ext_font_app.h"
#include "app_event.h"
//extern public_font_t interior_font_24_2bpp;
void lv_font_test_start(FONTTYPE_ENUM font)
{
    lv_obj_t* scr = lv_scr_act();
    lv_obj_t* label1 = lv_label_create(scr,NULL);
    static lv_style_t style1;
    lv_style_copy(&style1,&lv_style_plain_color);
    style1.text.font = (lv_font_t *)app_type_get_font(font); //&interior_font_24_2bpp;
    lv_label_set_style(label1,LV_LABEL_STYLE_MAIN,&style1);
    lv_label_set_text(label1,"A");
    lv_label_set_body_draw(label1,true);
    lv_obj_align(label1,NULL,LV_ALIGN_CENTER,0,0);
}

void LVGL_event_task(void *agrm)
{
    static FONTTYPE_ENUM font = e_HarmonyOS_Sans_SC_Medium_24_2Bpp;
    lv_font_test_start(font);
//    if(font < e_FontTypeMax)
//    {
//        font ++;
//    }
//    else
//    {
//        font = e_HarmonyOS_Sans_SC_Medium_16_1Bpp;
//    }
//    APP_EVENT_REMOVE_DELAY(LVGL_event_task);
//    APP_EVENT_PUSH_DELAY(LVGL_event_task, NULL, 2000);
}

static void lvgl_demo_run(void *p)
{
    lv_theme_t *th = lv_theme_material_init(210, LV_FONT_DEFAULT);
    LVGL_event_task((uint32_t *)e_HarmonyOS_Sans_SC_Medium_24_2Bpp);
    while(1)
    {
        rt_thread_delay(RT_TICK_PER_SECOND/100);
        lv_task_handler(); 
    }
} 

int rt_lvgl_demo_init(void)
{
    rt_err_t ret = RT_EOK;
    rt_thread_t thread = RT_NULL; 
    app_ext_font_init();
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
