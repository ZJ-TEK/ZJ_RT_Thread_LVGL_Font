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
#define font_printf(...) 	do{NRF_LOG_INFO(__VA_ARGS__);/*rt_kprintf(__VA_ARGS__);*/}while(0)
        
#define LCD_BL_EN   NRF_GPIO_PIN_MAP(0,  4)
static uint16_t lcd_buffer[LCD_IE3130B_WIDTH][ LCD_IE3130B_HEIGHT ];
public_font_t *my_font = NULL;

static uint16_t utf8_char_decode(const char *s, uint8_t *size)
{
    uint16_t r;
    uint8_t code = *(const uint8_t *)s;
    if ((code & 0x80) == 0) //0xxx xxxx
    {
        r = code;
        *size += 1;
    }
    else if ((code & 0xe0) == 0xc0) //110x xxxx 10xx xxxx
    {
        r = (code & 0x1f) << 6;
        code = *(++s);
        code &= 0x3f;
        r |= code;
        *size += 2;
    }
    else if ((code & 0xf0) == 0xe0) //111x xxxx 110x xxxx 10xx xxxx
    {
        r = (code & 0x0f) << 12;
        code = *(++s);
        code &= 0x3f;
        r |= (code << 6);
        code = *(++s);
        code &= 0x3f;
        r |= code;
        *size += 3;
    }
    else
    {
        printf("Illegal character during UTF-8 decoding!");
        r = 1;
        *size = 0;
    }
    return r;
}


void oled_go(int x_pos, int y_pos, char * str)
{
    nrf_gpio_cfg_output(LCD_BL_EN);
    nrf_gpio_pin_set(LCD_BL_EN);
//    NRF_LOG_INFO("strlen(str) = %d",strlen(str));
    uint8_t char_index = 0;
    while(1)
    {
        uint16_t unicode =  utf8_char_decode(&str[char_index], &char_index);
        
        glyph_dsc_t bitmap_dsc;
        
        my_font->get_glyph_dsc(my_font, &bitmap_dsc, unicode, 0);
        const uint8_t * glyph_bitmap =  my_font->get_glyph_bitmap(my_font, unicode);
        
        uint8_t bytes_pre_line = (bitmap_dsc.box_w * bitmap_dsc.bpp + 7) / 8;
        uint8_t bppEvtBytes = 8 / bitmap_dsc.bpp;
        
//        NRF_LOG_INFO("bitmap_dsc.adv_w = 0x%x",bitmap_dsc.adv_w);
//        NRF_LOG_INFO("bitmap_dsc.box_w = 0x%x",bitmap_dsc.box_w);
//        NRF_LOG_INFO("bitmap_dsc.box_h = 0x%x",bitmap_dsc.box_h);
//        NRF_LOG_INFO("bitmap_dsc.ofs_x = 0x%x",bitmap_dsc.ofs_x);
//        NRF_LOG_INFO("bitmap_dsc.ofs_y = %d",bitmap_dsc.ofs_y);
//        NRF_LOG_INFO("bitmap_dsc.bpp   = 0x%x",bitmap_dsc.bpp);
//        NRF_LOG_INFO("bytes_pre_line         = 0x%x",bytes_pre_line);
//        NRF_LOG_INFO("bppEvtBytes            = %d",bppEvtBytes);

//        NRF_LOG_INFO("(%d, %d)",x_pos,y_pos);
//        NRF_LOG_INFO("unicode = 0x%x",unicode);
        
//        NRF_LOG_HEXDUMP_INFO(glyph_bitmap+bytes_pre_line * bitmap_dsc.box_h/2, bytes_pre_line * bitmap_dsc.box_h / 2);
        
        
        uint8_t offsetmask[8] = {6, 4, 2, 0, 0, 0, 0, 0};
        uint8_t bits_mask = 0x03;
        if(bitmap_dsc.bpp == 1)
        { 
            bits_mask = 0x01;
            offsetmask[0] = 7;
            offsetmask[1] = 6;
            offsetmask[2] = 5;
            offsetmask[3] = 4;
            offsetmask[4] = 3;
            offsetmask[5] = 2;
            offsetmask[6] = 1;
            offsetmask[7] = 0;
        }
        else if(bitmap_dsc.bpp == 4)
        { 
            bits_mask = 0x0f;
            offsetmask[0] = 4;
            offsetmask[1] = 0;
        }
        
        uint8_t byte, gray_color;
        
        uint16_t bgcolor = 0, fgcolor = 0xffff;
        
        uint8_t bgr = (bgcolor >> 11) & 0x1F;             
        uint8_t bgg = (bgcolor >> 5) & 0x3F;             
        uint8_t bgb = bgcolor & 0x1F;
        uint8_t fgr = (fgcolor >> 11)& 0x1F;             
        uint8_t fgg = (fgcolor >> 5)& 0x3F;             
        uint8_t fgb = fgcolor & 0x1F;  
        
        
        
        
        uint16_t y_index = 0, x_index = 0;
        uint8_t bits = 0;
        bool loop = true;
        while(loop)
        {
            byte = *glyph_bitmap++;
            for(bits = 0; bits < bppEvtBytes; bits++) //bit2
            {
                if (x_index < bitmap_dsc.box_w)
                {
                    gray_color = (byte >> offsetmask[bits]) & bits_mask;
                    uint8_t alpha = 0;
                    
                    if(bitmap_dsc.bpp == 2)
                    {
                        if(gray_color == 0)
                        {
                            alpha = 0;
                        }
                        else if(gray_color == 1)
                        {
                            alpha = 33;
                        }
                        else if(gray_color == 2)
                        {
                            alpha = 66;
                        }
                        else if(gray_color == 3)
                        {
                            alpha = 100;
                        }
                    }
                    else if(bitmap_dsc.bpp == 4)
                    {
                        if(gray_color == 0)
                        {
                            alpha = 0;
                        }
                        else if(gray_color == 1)
                        {
                            alpha = 7;
                        }
                        else if(gray_color == 2)
                        {
                            alpha = 14;
                        }
                        else if(gray_color == 3)
                        {
                            alpha = 21;
                        }
                        else if(gray_color == 4)
                        {
                            alpha = 28;
                        }
                        else if(gray_color == 5)
                        {
                            alpha = 35;
                        }
                        else if(gray_color == 6)
                        {
                            alpha = 42;
                        }
                        else if(gray_color == 7)
                        {
                            alpha = 49;
                        }
                        else if(gray_color == 8)
                        {
                            alpha = 56;
                        }
                        else if(gray_color == 9)
                        {
                            alpha = 63;
                        }
                        else if(gray_color == 10)
                        {
                            alpha = 70;
                        }
                        else if(gray_color == 11)
                        {
                            alpha = 77;
                        }
                        else if(gray_color == 12)
                        {
                            alpha = 84;
                        }
                        else if(gray_color == 13)
                        {
                            alpha = 91;
                        }
                        else if(gray_color == 14)
                        {
                            alpha = 98;
                        }
                        else if(gray_color == 15)
                        {
                            alpha = 100;
                        }
                    }
                    else
                    {
                        if(gray_color == 0)
                        {
                            alpha = 0;
                        }
                        else if(gray_color == 1)
                        {
                            alpha = 100;
                        }
                    }
                    
                    
                    uint8_t newfgr = ((bgr * alpha +  (fgr * (100 - alpha))) / 100) & 0x1F;           
                    uint8_t newfgg = ((bgg * alpha +  (fgg * (100 - alpha))) / 100) & 0x3F;            
                    uint8_t newfgb = ((bgb * alpha +  (fgb * (100 - alpha))) / 100) & 0x1F; 
        
                    lcd_buffer[x_pos + bitmap_dsc.ofs_x + x_index][y_pos + bitmap_dsc.ofs_y + y_index] = newfgr << 11 | newfgg << 5 | newfgb;
                }
                x_index++;
                if(x_index == bytes_pre_line * 8 / bitmap_dsc.bpp)
                {
                    x_index = 0;
                    y_index++;
                    if(y_index == bitmap_dsc.box_h)
                    {
                        loop = false;
                    }
                }
            }
        }//while(loop)
        x_pos += bitmap_dsc.adv_w;
        if(char_index >= str[char_index])
          break;
    }
    lcd_ie3130b_put_gfx(0, 0, LCD_IE3130B_WIDTH, LCD_IE3130B_HEIGHT, (uint8_t *)lcd_buffer);
}

int main(void)
{   
    app_ext_font_init();
    APP_EVENT_PUSH_DELAY(led_event_start,NULL,500);
    NRF_LOG_INFO("RT-Thread for nrf52840 started");
    memset(lcd_buffer, 0xff , sizeof(lcd_buffer));
    my_font = app_type_get_font(e_HarmonyOS_Sans_SC_Medium_24_2Bpp);
    oled_go(50,50, "支持RLE和HASH");
    my_font = app_type_get_font(e_fHarmonyOS_Sans_SC_Medium_24_2Bpp);
    oled_go(50,100, "支持RLE和HASH");
    my_font = app_type_get_font(e_fake_font);
    oled_go(50,150, "Asdfg");
    my_font = app_type_get_font(e_interior_font_HarmonyOS_Sans_SC_Medium_24_2bpp);
    oled_go(50,200, "Asdfg");

//    oled_go(50,150, "藍牙\n");
//    oled_go(50,150, "支持RLE和HASH");
    return RT_TRUE;
}


/**
 * @}
 */
