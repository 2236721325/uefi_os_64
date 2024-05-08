#pragma once
#include "os/kernel/types.h"
#define LAYER_MAX_COUNT 256
typedef struct 
{
    uint32_t* buffer;
    int width;
    int height;
    int x;
    int y;
    int color_invisible;
    int flags;
}Layer;
typedef struct
{
    uint32_t* vga_base;
    int width;
    int height;
    int top_layer;
    Layer layers[LAYER_MAX_COUNT];
}LayerControl;

void gui_init();