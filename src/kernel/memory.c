#include "os/kernel/memory.h"
#include "os/kernel/efi_info.h"
#include "os/kernel/global.h"
#include "os/kernel/draw.h"
    
    //1:RAM
    //2:ROM or Reserved
    //3:ACPI Reclaim Memory
    //4:ACPI NVS Memory
    //5:Unusable



extern char _end;


#define PAGE_BYTE_SIZE 0x200000   //2mb


static bool is_kernel_in(uint32_t block_idx)
{
    if(
        g_memory_manager.memory_block[block_idx].phy_addr_start<=0x100000 &&
         g_memory_manager.memory_block[block_idx].phy_addr_start+g_memory_manager.memory_block[block_idx].length>0x100000
    )
    {
        return true;
    }
    return false;
}
void memory_init()
{
    uint64_t total_memory=0;
    g_memory_manager.block_count=0;
    for ( uint32_t i = 0; i < tmp_efiInfo->memoryInfo.E820_Entry_count; i++)
    {
        
        EFI_E820_MEMORY_DESCRIPTOR* descriptor=&(tmp_efiInfo->memoryInfo.E820_Entry[i]);
        if (descriptor->type==1&&descriptor->length>=1024*1024*2)
        {
            total_memory+=descriptor->length;
            g_memory_manager.memory_block[g_memory_manager.block_count].phy_addr_start=descriptor->address;
            g_memory_manager.memory_block[g_memory_manager.block_count].length=descriptor->length;
            g_memory_manager.block_count++;

        }
        draw_printf(0,128+16*i,COLOR_WHITE,"Address:%X,Length:%X byte <=>%d mb,Type:%x",
                    descriptor->address,descriptor->length,descriptor->length/1024/1024,descriptor->type);
    }
    draw_printf(0,112,COLOR_WHITE,"Total Physical Memory->%X byte <=> %d mb",total_memory,total_memory/1024/1024);



    for (uint32_t i = 0; i < g_memory_manager.block_count; i++)
    {
        draw_printf(512,128+16*i,COLOR_WHITE,"MemoryBlock->Address:%X,Length:%X byte <=>%d mb",
                    g_memory_manager.memory_block[i].phy_addr_start,g_memory_manager.memory_block[i].length,g_memory_manager.memory_block[i].length/1024/1024);
    }
    
    void* kernel_end_ptr=(void*)&_end;
    uint8_t* bitmap_start_ptr=(uint8_t*)kernel_end_ptr;
    for (uint32_t i = 0; i < g_memory_manager.block_count; i++)
    {
        uint64_t address=g_memory_manager.memory_block[i].phy_addr_start;
        uint64_t length=g_memory_manager.memory_block[i].length;
        Bitmap* bmp=&g_memory_manager.memory_block[i].bitmap;
        bmp->bytes_len=length/PAGE_BYTE_SIZE;
        bmp->bits=bitmap_start_ptr;
        bitmap_init(bmp);
        bitmap_start_ptr=bitmap_start_ptr+bmp->bytes_len;
    }
    return ;
}