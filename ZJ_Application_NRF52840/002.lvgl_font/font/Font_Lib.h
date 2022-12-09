/* * This file is generated by py_FontMaker generation tools
 * Please DO NOT modify this file
 *
 * author: John
 * github: 
 * Generated at 2022-12-07 13:50:30
 * Version: 0x91fcc446
 */

#ifndef _FONT_LIB_H_
#define _FONT_LIB_H_
#include "stdint.h"
#include "stdbool.h"
#include "string.h"
#define FONT_LIB_VERSION               0x91fcc446
#define FONT_TYPE_NUMBER               2


/** use LVGL, you need to set LVGL_FONT_ENABLE macro to 1 */
#define LVGL_FONT_ENABLE               0


/** CMAP search use HASH, you need to set CMAP_HASH_ENABLE macro to 1 */
#define CMAP_HASH_ENABLE               1


/*CMAP use cache for font*/
#define EXT_FONT_DSC_CACHE_ENABLE      1
#define EXT_FONT_DSC_CACHE_CNT         (100) //The default cache is 100 CMAP,Users can modify it according to their memory and requirements


/** RLE enable(DO NOT modify,It can only be modified by py_FontMaker), You have to regenerate Font_lib.bin from py_FontMaker and update it to your device */
#define BITMAP_RLE_ENCODE_ENABLE       1


#define MAX_STATIC_BITMAP_BUF_SIZE     (3200)	//Please calculate according to the maximum font (width * height * bpp)/8


#pragma pack(1)
// For sparse glyph cmap table
typedef struct
{
    uint16_t code_point;
    uint32_t offset;
    uint16_t size;
}cmap_sparse_table_t;

// For continous glyph cmap table
typedef struct
{
    uint32_t offset;
    uint16_t size;
}cmap_continous_table_t;

//bitmap description
typedef struct
{
    uint8_t x_advance;
    uint8_t x_size;
    uint8_t y_size;
    int8_t  x_off;
    int8_t  y_off;
    uint8_t bytes_pre_line;
} char_bitmap_attribute_t;

// Font information
typedef struct
{
    char *   font_name;
    uint8_t  bpp;
    uint8_t  cmap_continued;           // 1 continous 0 sparse
    uint16_t y_size;
    uint16_t y_dist;
    uint16_t baseline;
    uint16_t l_hight;
    uint16_t c_hight;
    uint16_t first_code;
    uint16_t last_code;
    uint32_t cmap_offset;
    uint16_t code_num;
    cmap_sparse_table_t* p_cmap_table;  // if cmap_continued = 0 this vilue will be set
}font_type_info_t;
#pragma pack()


#if LVGL_FONT_ENABLE
#include "lvgl.h"
#define public_font_t lv_font_t
#define glyph_dsc_t lv_font_glyph_dsc_t
#else
/** Describes the properties of a glyph.*/
typedef struct
{
    uint16_t adv_w; /**< The glyph needs this space. Draw the next glyph after this width.*/
    uint16_t box_w; /**< Width of the glyph's bounding box*/
    uint16_t box_h; /**< Height of the glyph's bounding box*/
    int16_t ofs_x;  /**< x offset of the bounding box*/
    int16_t ofs_y;  /**< y offset of the bounding box*/
    uint8_t bpp;    /**< Bit-per-pixel: 1, 2, 4*/
} glyph_dsc_t;

/*Describe the properties of a font*/
typedef struct _public_font_struct
{
    /** Get a glyph's  descriptor from a font*/
    bool (*get_glyph_dsc)(const struct _public_font_struct *, glyph_dsc_t *, uint32_t letter, uint32_t letter_next);

    /** Get a glyph's bitmap from a font*/
    const uint8_t * (*get_glyph_bitmap)(const struct _public_font_struct *, uint32_t);

    /*Pointer to the font in a font pack (must have the same line height)*/
    int16_t line_height;      /**< The real line height where any text fits*/
    int16_t base_line;        /**< Base line measured from the top of the line_height*/
    void * user_data;         /**< Custom user data for font. */
} public_font_t;
#endif

/*******************************************************************************************************/
// fake font declaration
extern public_font_t fake_font;    //any error will use fake font
/*******************************************************************************************************/


extern public_font_t HarmonyOS_Sans_SC_Medium_24_2Bpp;
extern public_font_t fHarmonyOS_Sans_SC_Medium_24_2Bpp;


/***********************************************************
 *       You can declare internal font below
 ***********************************************************/
extern public_font_t interior_font_HarmonyOS_Sans_SC_Medium_24_2bpp;


extern const font_type_info_t HarmonyOS_Sans_SC_Medium_24_2Bpp_font_info;
extern const font_type_info_t fHarmonyOS_Sans_SC_Medium_24_2Bpp_font_info;


#endif

