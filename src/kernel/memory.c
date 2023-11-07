#include "os/kernel/memory.h"
#include "os/kernel/efi_info.h"
#include "os/kernel/global.h"
#include "os/kernel/draw.h"
#include "os/kernel/stdio.h"
#include "os/lib/bitmap.h"
    //1:RAM
    //2:ROM or Reserved
    //3:ACPI Reclaim Memory
    //4:ACPI NVS Memory
    //5:Unusable

extern char _end;




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

    uint64_t highest_memory=0;
    for ( uint32_t i = 0; i < tmp_efiInfo->memoryInfo.E820_Entry_count; i++)
    {
        
        EFI_E820_MEMORY_DESCRIPTOR* descriptor=&(tmp_efiInfo->memoryInfo.E820_Entry[i]);
        if (descriptor->type==1&&descriptor->length>=1024*1024*2)
        {
            total_memory+=descriptor->length;
            g_memory_manager.memory_block[g_memory_manager.block_count].phy_addr_start=descriptor->address;
            g_memory_manager.memory_block[g_memory_manager.block_count].length=descriptor->length;
            g_memory_manager.block_count++;
            if(descriptor->address+descriptor->length>highest_memory)
            {
                highest_memory=descriptor->address+descriptor->length;
            }

        }
        draw_printf(0,128+16*i,COLOR_WHITE,"Address:%X,Length:%X byte <=>%d mb,Type:%x",
                    descriptor->address,descriptor->length,descriptor->length/1024/1024,descriptor->type);
    }
    draw_printf(0,112,COLOR_WHITE,"Total Physical Memory->%X byte <=> %d mb",total_memory,total_memory/1024/1024);
    draw_printf(0,96,COLOR_WHITE,"Highest Physical Memory->%X byte <=> %d mb",highest_memory,highest_memory/1024/1024);

    for (uint32_t i = 0; i < g_memory_manager.block_count; i++)
    {
        draw_printf(512,128+16*i,COLOR_WHITE,"MemoryBlock->Address:%X,Length:%X byte <=>%d mb",
                    g_memory_manager.memory_block[i].phy_addr_start,g_memory_manager.memory_block[i].length,g_memory_manager.memory_block[i].length/1024/1024);
    }
    
    void* kernel_end_ptr=(void*)&_end;
    uint8_t* address_ptr=(uint8_t*)kernel_end_ptr;

    //init_bitmap
    for (uint32_t i = 0; i < g_memory_manager.block_count; i++)
    {
        uint64_t address=g_memory_manager.memory_block[i].phy_addr_start;
        uint64_t length=g_memory_manager.memory_block[i].length;
        Bitmap* bmp=&g_memory_manager.memory_block[i].bitmap;
        
        bmp->bytes_len=DIV_ROUND_UP(length/PDE_BYTE_SIZE,8);
        bmp->last_byte_space=(length/PDE_BYTE_SIZE)%8;
        bmp->bits=address_ptr;
        bitmap_init(bmp);
        address_ptr+=bmp->bytes_len;
        if(is_kernel_in(i))
        {
            for (uint32_t i = 0; i < Kernel_Reserved_Page_Count; i++)
            {
                bitmap_set(bmp,i,1);
            }
        }
    }
    //init_kernel_virtual_address
    g_kernel_vaddr.vaddr_bitmap.bytes_len=DIV_ROUND_UP(total_memory/PDE_BYTE_SIZE,8);
    g_kernel_vaddr.vaddr_bitmap.last_byte_space=(total_memory/PDE_BYTE_SIZE)%8;
    g_kernel_vaddr.vaddr_bitmap.bits=address_ptr;
    g_kernel_vaddr.vaddr_start=K_HEAP_START;
    address_ptr+=g_kernel_vaddr.vaddr_bitmap.bytes_len;
    
    bitmap_init(&g_kernel_vaddr.vaddr_bitmap);

    uint32_t page_2mb_count=DIV_ROUND_UP(highest_memory,PDPTE_BYTE_SIZE);
    address_ptr--;
    address_ptr=(uint8_t*)(((uint64_t)address_ptr)&0xfffffffffffff000);
    address_ptr+=PAGE_TABLE_SIZE;


    for (uint32_t i =1 ; i < page_2mb_count; i++)
    {
        set_PDPTE(i,virtual_address_to_physical((uint64_t)address_ptr));
        memsetl(address_ptr,0,512);
        address_ptr+=PAGE_TABLE_SIZE;
    }
    for (uint32_t i = 0; i < 100; i++)
    {
        void* test=phy_alloc_2m();
    }
    
    

    return ;
}



void set_PDPTE(uint32_t idx,uint64_t pde_address)
{
    uint64_t * ptr = (uint64_t *)PDPT_ADDRESS;
    ptr+=idx;
    *ptr=(pde_address)|0x7;
}

uint64_t virtual_address_to_physical(uint64_t virtual_address)
{
    return virtual_address-0xffff800000000000;
}



void* phy_alloc_2m()
{
    uint32_t count=g_memory_manager.block_count;
    for (uint32_t i = 0; i < count; i++)
    {
        int32_t idx=bitmap_scan(&g_memory_manager.memory_block[i].bitmap,1);
        if(idx!=-1)
        {
            bitmap_set(&g_memory_manager.memory_block[i].bitmap,idx,1);
            return (void*) (g_memory_manager.memory_block[i].phy_addr_start+idx*PDE_BYTE_SIZE);
        }
    }
    return NULL;
}