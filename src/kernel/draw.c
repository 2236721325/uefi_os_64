
#include "os/kernel/draw.h"
#include "os/kernel/font.h"
#include "os/kernel/stdio.h"

#include <stdarg.h>

void draw_pixel(uint32_t x, uint32_t y, Color color)
{
    uint32_t hr = g_graphicInfo->PixelsPerScanLine;
    Color *base = (Color *)g_graphicInfo->FrameBufferBase;
    Color *pixel = base + (hr * y) + x;
    *pixel=color;
}

void draw_straight_line(uint32_t x, uint32_t y,uint32_t len,Color color)
{
    void *base = (void *)g_bootInfo.graphicInfo.FrameBufferBase + (y * g_graphicInfo->PixelsPerScanLine + x)*4;
    uint64_t value =0;
    value |= (uint64_t)color.Blue;
    value |= (uint64_t)color.Green <<8;
    value |= (uint64_t)color.Red << 16;
    value |= (uint64_t)color.Reserved <<14;
    value |=value<<32;
    memsetl(base,value,len/2);
    if(len%2)
    {
        draw_pixel(x+len-1,y,color);
    }
}

// -------x
// |
// |
// |
// y
void draw_rect(Rect rect, Color color)
{
    
    uint32_t i;
    for (i = rect.X; i < (rect.X + rect.Width); i++)
    {
        draw_pixel(i, rect.Y, color);
        draw_pixel(i, rect.Y + rect.Height - 1, color);
    }

    for (i = rect.Y; i < (rect.Y + rect.Height); i++)
    {
        draw_pixel(rect.X, i, color);
        draw_pixel(rect.X + rect.Width - 1, i, color);
    }
}
void draw_solid_rect(Rect rect, Color color)
{
    
    
    for (uint32_t y = rect.Y; y< (rect.Y + rect.Height); y++)
    {
        draw_straight_line(rect.X,y,rect.Width,color);
    }

  
}

void clear_screen(Color color)
{
    void *base = (void *)g_graphicInfo->FrameBufferBase;
    uint64_t value =0;
    value |= (uint64_t)color.Blue;
    value |= (uint64_t)color.Green <<8;
    value |= (uint64_t)color.Red << 16;
    value |= (uint64_t)color.Reserved <<14;
    value |=value<<32;
    memsetl(base,value,g_bootInfo.graphicInfo.HorizontalResolution*g_bootInfo.graphicInfo.VerticalResolution/2);
}


void draw_font_ascii(uint32_t x,uint32_t y,Color color,uint8_t c)
{
    for (uint32_t i = 0; i < 16; i++)
    {
        uint8_t data = g_font_ascii[c][i];
        if ((data & 0x80) != 0)
        {
            draw_pixel(x, y + i, color);
        }
        if ((data & 0x40) != 0)
        {
            draw_pixel(x + 1, y + i, color);
        }
        if ((data & 0x20) != 0)
        {
            draw_pixel(x + 2, y + i, color);
        }
        if ((data & 0x10) != 0)
        {
            draw_pixel(x + 3, y + i, color);
        }
        if ((data & 0x08) != 0)
        {
            draw_pixel(x + 4, y + i, color);
        }
        if ((data & 0x04) != 0)
        {
            draw_pixel(x + 5, y + i, color);
        }
        if ((data & 0x02) != 0)
        {
            draw_pixel(x + 6, y + i, color);
        }
        if ((data & 0x01) != 0)
        {
            draw_pixel(x + 7, y + i, color);
        }
    }
    return;
}

void draw_text(uint32_t x, uint32_t y,Color color, uint8_t *str)
{
    for (uint32_t i = 0; str[i] != 0x00; i++)
    {
        draw_font_ascii(x + i * 8, y ,color,str[i]);
    }
}

uint32_t draw_printf(uint32_t x, uint32_t y, Color color, const uint8_t *fmt, ...)
{
    va_list args;
    va_start(args, fmt); // 使args指向format
    uint32_t len=vsprintf(g_buffer, fmt, args);
    va_end(args);
    draw_text(x,y,color,g_buffer);
    return len;
}
void init_draw()
{
    g_graphicInfo=&g_bootInfo.graphicInfo;
    clear_screen((Color){100,100,100,0});
}
void draw_test()
{
    // Rect rect = {0, 10, 100, 200}; /* 新增 */
    // Color color = {0, 0, 255, 0};
    // draw_rect(rect, color);
    // Color color2 = {255, 255, 255, 0};
    // draw_printf(500,500,color2,"HelloWorld->%x",1234);
}