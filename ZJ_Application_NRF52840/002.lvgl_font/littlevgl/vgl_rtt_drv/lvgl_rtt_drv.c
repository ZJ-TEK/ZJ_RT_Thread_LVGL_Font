#include "lvgl_rtt_drv.h" 
#include <rtdevice.h> 
#include "lvgl.h" 

static rt_device_t device; 
static struct rt_device_graphic_info info; 
static struct rt_messagequeue *input_mq; 

static void lvgl_rtt_lcd_flush_cb(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    /*Return if the area is out the screen*/
    if(area->x2 < 0) return;
    if(area->y2 < 0) return;
    if(area->x1 > info.width  - 1) return;
    if(area->y1 > info.height - 1) return;

    /*Truncate the area to the screen*/
    int32_t act_x1 = area->x1 < 0 ? 0 : area->x1;
    int32_t act_y1 = area->y1 < 0 ? 0 : area->y1;
    int32_t act_x2 = area->x2 > info.width  - 1 ? info.width  - 1 : area->x2;
    int32_t act_y2 = area->y2 > info.height - 1 ? info.height - 1 : area->y2;

    uint32_t x; 
    uint32_t y; 
    long int location = 0;

    /* 8 bit per pixel */
    if(info.bits_per_pixel == 8)
    {
        uint8_t *fbp8 = (uint8_t*)info.framebuffer;
        
        for(y = act_y1; y <= act_y2; y++) 
        {
            for(x = act_x1; x <= act_x2; x++) 
            {
                location = (x) + (y) * info.width;
                fbp8[location] = color_p->full;
                color_p++;
            }

            color_p += area->x2 - act_x2;
        }
    }
    
    /* 16 bit per pixel */ 
    else if(info.bits_per_pixel == 16)
    {
        uint16_t *fbp16 = (uint16_t*)info.framebuffer;
        
        for(y = act_y1; y <= act_y2; y++) 
        {
            for(x = act_x1; x <= act_x2; x++) 
            {
                location = (x) + (y) * info.width;
                fbp16[location] = color_p->full;
                color_p++;
            }

            color_p += area->x2 - act_x2;
        }
    }
        
    /* 24 or 32 bit per pixel */
    else if(info.bits_per_pixel == 24 || info.bits_per_pixel == 32)
    {
        uint32_t *fbp32 = (uint32_t*)info.framebuffer;
        
        for(y = act_y1; y <= act_y2; y++) 
        {
            for(x = act_x1; x <= act_x2; x++) 
            {
                location = (x) + (y) * info.width;
                fbp32[location] = color_p->full;
                color_p++;
            }

            color_p += area->x2 - act_x2;
        }
    }
    
    struct rt_device_rect_info rect_info; 

    rect_info.x = area->x1;
    rect_info.y = area->y1;
    rect_info.width = area->x2 - area->x1;
    rect_info.height = area->y2 - area->y1;
    rt_device_control(device, RTGRAPHIC_CTRL_RECT_UPDATE, &rect_info);

    lv_disp_flush_ready(disp_drv);
}

static rt_bool_t touch_down = RT_FALSE;
static rt_int16_t last_x = 0;
static rt_int16_t last_y = 0;

static bool lvgl_rtt_input_read(lv_indev_data_t *data) 
{
    data->point.x = last_x;
    data->point.y = last_y;
    data->state = (touch_down == RT_TRUE) ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;

    return false;
}

static void lvgl_rtt_tick_run(void *p)
{
    while(1)
    {
        lv_tick_inc(1); 
        rt_thread_delay(1);
    }
} 

void lvgl_rtt_send_input_event(rt_int16_t x, rt_int16_t y, rt_uint8_t state) 
{
    switch(state)
    {
    case LVGL_RTT_INPUT_UP:
        touch_down = RT_FALSE;
        break; 
    case LVGL_RTT_INPUT_DOWN:
        last_x = x;
        last_y = y; 
        touch_down = RT_TRUE; 
        break; 
    case LVGL_RTT_INPUT_MOVE:
        last_x = x;
        last_y = y;
        break; 
    }
}

#if USE_LV_LOG
void lvgl_rtt_log_register(lv_log_level_t level, const char * file, uint32_t line, const char * dsc)
{
    if(level >= LV_LOG_LEVEL) {
      //Show the log level if you want
      if(level == LV_LOG_LEVEL_TRACE)  {
         rt_kprintf("Trace:");
      }

      rt_kprintf("%s\n", dsc);
      //You can write 'file' and 'line' too similary if required.
    }
}
#endif

rt_err_t lvgl_rtt_init(const char *name) 
{
    RT_ASSERT(name != RT_NULL); 
    
    /* LCD Device Init */
    device = rt_device_find(name); 
    RT_ASSERT(device != RT_NULL);
    if(rt_device_open(device, RT_DEVICE_OFLAG_RDWR) == RT_EOK)
    {
        rt_device_control(device, RTGRAPHIC_CTRL_GET_INFO, &info);
    }
    
    RT_ASSERT(info.bits_per_pixel ==  8 || info.bits_per_pixel == 16 || 
              info.bits_per_pixel == 24 || info.bits_per_pixel == 32); 

    if((info.bits_per_pixel != LV_COLOR_DEPTH) && (info.bits_per_pixel != 32 && LV_COLOR_DEPTH != 24))
    {
        rt_kprintf("Error: framebuffer color depth mismatch! (Should be %d to match with LV_COLOR_DEPTH)", 
            info.bits_per_pixel); 
        return RT_ERROR; 
    }

    /* littlevgl Init */ 
    lv_init(); 

#if USE_LV_LOG
    /* littlevgl Log Init */ 
    lv_log_register_print(lvgl_rtt_log_register);
#endif
    /* Example for 1) */
    static lv_disp_buf_t disp_buf;
    lv_disp_buf_init(&disp_buf, info.framebuffer, NULL, LV_HOR_RES_MAX * LV_VER_RES_MAX);   /*Initialize the display buffer*/

    /* littlevGL Display device interface */
    lv_disp_drv_t disp_drv; 
    lv_disp_drv_init(&disp_drv); 
    
    /*Set the resolution of the display*/
    disp_drv.hor_res = info.height;
    disp_drv.ver_res = info.width;

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = lvgl_rtt_lcd_flush_cb;

    /*Set a display buffer*/
    disp_drv.buffer = &disp_buf;

    lv_disp_drv_register(&disp_drv); 
    
    /* littlevGL Tick thread */ 
    rt_thread_t thread = RT_NULL; 

    thread = rt_thread_create("lv_tick", lvgl_rtt_tick_run, RT_NULL, 512, 6, 10); 
    if(thread == RT_NULL)
    {
        return RT_ERROR; 
    }
    rt_thread_startup(thread); 
 
    return RT_EOK; 
}
