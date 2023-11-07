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

typedef struct {
   Bitmap vaddr_bitmap;      // 虚拟地址用到的位图结构 
   uint64_t vaddr_start;            // 虚拟地址起始地址
}VirtualPool;

MemoryManager g_memory_manager;
VirtualPool g_kernel_vaddr;

#define PDE_BYTE_SIZE 0x200000   //2mb

#define PDPTE_BYTE_SIZE 0x40000000 //1GB


#define PAGE_TABLE_SIZE 0x1000 //4096
#define Kernel_Reserved_Page_Count 3
#define PDPT_ADDRESS 0xffff800000102000

#define K_HEAP_START 0xffff800001000000


uint64_t virtual_address_to_physical(uint64_t virtual_address);

void memory_init();


void set_PDPTE(uint32_t idx,uint64_t pde_address);
void* phy_alloc_2m();

void vir_alloc();