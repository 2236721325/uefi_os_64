#pragma once


#include "os/kernel/types.h"
#include "os/lib/ioqueue.h"

#define MOUSEBUFFER_SIZE 64

struct ioqueue g_mousequeue;

uint8_t g_mousebuf[MOUSEBUFFER_SIZE];

int32_t g_mouse_x;
int32_t g_mouse_y;

struct MouseCode
{
    int32_t x;
    int32_t y;
    uint8_t btn;
    uint8_t buffer[3];
    int phase;
};

struct MouseCode mouse_code_g;

void mouse_init();

void move_mouse(int x,int y);
void draw_mouse();