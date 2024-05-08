#pragma once
#include "os/kernel/boot_info.h"
#include "os/kernel/types.h"

// 128TB
#define VGA_TEMP_VIRTUAL_ADDRESS 0xffff800003000000L

struct Color {
    uint8_t Blue;
    uint8_t Green;
    uint8_t Red;
    uint8_t Reserved;
};

struct Rect {
    uint32_t X;
    uint32_t Y;
    uint32_t Width;
    uint32_t Height;
};

struct graphic_info graphicInfo_g;

#define COLOR_BLACK \
    (struct Color) { 0, 0, 0, 0 }
#define COLOR_RED \
    (struct Color) { 0, 0, 255, 0 }
#define COLOR_WHITE \
    (struct Color) { 255, 255, 255, 0 }

void draw_pixel(uint32_t x, uint32_t y, struct Color color);
void draw_rect(struct Rect rect, struct Color color);
void draw_straight_line(uint32_t x, uint32_t y, uint32_t len, struct Color color);
void draw_solid_rect(struct Rect rect, struct Color color);
void draw_font_ascii(uint32_t x, uint32_t y, struct Color color, uint8_t c);
uint32_t draw_printf(uint32_t x, uint32_t y, struct Color color, const uint8_t *fmt, ...);
void draw_text(uint32_t x, uint32_t y, struct Color color, uint8_t *str);

void draw_init();
