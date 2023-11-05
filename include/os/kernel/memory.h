#pragma once
#include "os/kernel/global.h"
#include "os/lib/bitmap.h"
typedef struct 
{
    Bitmap bitmap;
    uint64_t phy_addr_start;
    uint64_t length;
}MemoryBlock;

typedef struct
{
    uint32_t block_count;
    MemoryBlock memory_block[32];
}MemoryManager;


MemoryManager g_memory_manager;

void memory_init();