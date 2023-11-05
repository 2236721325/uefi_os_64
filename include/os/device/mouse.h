#pragma once


#include "os/kernel/types.h"
#include "os/lib/ioqueue.h"

#define MOUSEBUFFER_SIZE 64

ioqueue g_mousequeue;

uint8_t g_mousebuf[MOUSEBUFFER_SIZE];

uint32_t g_mouse_x;
uint32_t g_mouse_y;

void mouse_init();

void draw_mouse();