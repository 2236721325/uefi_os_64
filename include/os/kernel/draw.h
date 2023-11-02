#pragma once
#include "os/kernel/efi_info.h"
#include "os/kernel/types.h"



typedef struct 
{
    uint8_t Blue;
    uint8_t Green;
    uint8_t Red;
    uint8_t Reserved;
}Color;



typedef struct
{
    uint32_t X;
    uint32_t Y;
    uint32_t Width;
    uint32_t Height;
}Rect;



GraphicInfo* g_graphicInfo;


#define COLOR_BLACK (Color){0,0,0,0}
#define COLOR_RED (Color){0,0,255,0}


void draw_pixel(uint32_t x,uint32_t y,Color color);


void draw_rect(Rect rect,Color color);
void draw_straight_line(uint32_t x, uint32_t y,uint32_t len,Color color);
void draw_solid_rect(Rect rect, Color color);
void draw_font_ascii(uint32_t x,uint32_t y,Color color,uint8_t c);
uint32_t draw_printf(uint32_t x, uint32_t y, Color color, const uint8_t *fmt, ...);
void draw_text(uint32_t x, uint32_t y,Color color, uint8_t *str);


void init_draw();
void draw_test();