
#include "os/kernel/draw.h"
#include "os/kernel/font.h"
#include "os/lib/stdio.h"
#include "os/lib/string.h"
#include <stdarg.h>

void draw_pixel(uint32_t x, uint32_t y, struct Color color)
{
    uint32_t hr = graphicInfo_g.pixelsPerScanLine;
    struct Color *base = (struct Color *)graphicInfo_g.frameBufferBase;
    struct Color *pixel = base + (hr * y) + x;
    *pixel = color;
}

void draw_straight_line(uint32_t x, uint32_t y, uint32_t len, struct Color color)
{
    void *base = (void *)graphicInfo_g.frameBufferBase + (y * graphicInfo_g.pixelsPerScanLine + x) * 4;
    uint64_t value = 0;
    value |= (uint64_t)color.Blue;
    value |= (uint64_t)color.Green << 8;
    value |= (uint64_t)color.Red << 16;
    value |= (uint64_t)color.Reserved << 14;
    value |= value << 32;
    memsetl(base, value, len / 2);
    if (len % 2) {
        draw_pixel(x + len - 1, y, color);
    }
}

// -------x
// |
// |
// |
// y
void draw_rect(struct Rect rect, struct Color color)
{

    uint32_t i;
    for (i = rect.X; i < (rect.X + rect.Width); i++) {
        draw_pixel(i, rect.Y, color);
        draw_pixel(i, rect.Y + rect.Height - 1, color);
    }

    for (i = rect.Y; i < (rect.Y + rect.Height); i++) {
        draw_pixel(rect.X, i, color);
        draw_pixel(rect.X + rect.Width - 1, i, color);
    }
}
void draw_solid_rect(struct Rect rect, struct Color color)
{

    for (uint32_t y = rect.Y; y < (rect.Y + rect.Height); y++) {
        draw_straight_line(rect.X, y, rect.Width, color);
    }
}

void clear_screen(struct Color color)
{
    void *base = (void *)graphicInfo_g.frameBufferBase;
    uint64_t value = 0;
    value |= (uint64_t)color.Blue;
    value |= (uint64_t)color.Green << 8;
    value |= (uint64_t)color.Red << 16;
    value |= (uint64_t)color.Reserved << 14;
    value |= value << 32;
    memsetl(base, value, graphicInfo_g.horizontalResolution * graphicInfo_g.verticalResolution / 2);
}

void draw_font_ascii(uint32_t x, uint32_t y, struct Color color, uint8_t c)
{
    for (uint32_t i = 0; i < 16; i++) {
        uint8_t data = g_font_ascii[c][i];
        if ((data & 0x80) != 0) {
            draw_pixel(x, y + i, color);
        }
        if ((data & 0x40) != 0) {
            draw_pixel(x + 1, y + i, color);
        }
        if ((data & 0x20) != 0) {
            draw_pixel(x + 2, y + i, color);
        }
        if ((data & 0x10) != 0) {
            draw_pixel(x + 3, y + i, color);
        }
        if ((data & 0x08) != 0) {
            draw_pixel(x + 4, y + i, color);
        }
        if ((data & 0x04) != 0) {
            draw_pixel(x + 5, y + i, color);
        }
        if ((data & 0x02) != 0) {
            draw_pixel(x + 6, y + i, color);
        }
        if ((data & 0x01) != 0) {
            draw_pixel(x + 7, y + i, color);
        }
    }
    return;
}

void draw_text(uint32_t x, uint32_t y, struct Color color, uint8_t *str)
{
    for (uint32_t i = 0; str[i] != 0x00; i++) {
        draw_font_ascii(x + i * 8, y, color, str[i]);
    }
}

uint32_t draw_printf(uint32_t x, uint32_t y, struct Color color, const uint8_t *fmt, ...)
{
    memsetl(vsprintf_buffer_g, 0, 512);
    va_list args;
    va_start(args, fmt); // 使args指向format
    uint32_t len = vsprintf(vsprintf_buffer_g, fmt, args);
    va_end(args);
    draw_text(x, y, color, vsprintf_buffer_g);
    return len;
}
void draw_init()
{
    graphicInfo_g = bootInfo_g->graphicInfo;
    graphicInfo_g.frameBufferBase = 0xffff800003000000;
    clear_screen((struct Color){100, 100, 100, 0});
}
