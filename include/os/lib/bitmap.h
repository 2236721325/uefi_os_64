#pragma once

#include "os/kernel/global.h"


struct Bitmap
{
    uint32_t bytes_len;
    uint32_t last_byte_space;
    uint8_t* bits;
};


void bitmap_init(struct Bitmap* bitmap);

bool bitmap_scan_test(struct Bitmap* bitmap,uint32_t bit_idx);

void bitmap_set(struct Bitmap* bitmap,uint32_t bit_idx,int8_t value);


//用来在位图中找到cnt个连续的0，以此来分配一块连续未被占用的内存，参数有指向位图的指针与要分配的内存块的个数cnt
//成功就返回起始位的偏移（如果把位图看做一个数组，那么也可以叫做下标），不成功就返回-1
int bitmap_scan(struct Bitmap* btmp, uint32_t cnt);