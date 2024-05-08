#include "os/kernel/memory.h"
#include "os/kernel/boot_info.h"
#include "os/kernel/debug.h"
#include "os/kernel/draw.h"
#include "os/kernel/global.h"
#include "os/lib/bitmap.h"
#include "os/lib/stdio.h"
// 1:RAM
// 2:ROM or Reserved
// 3:ACPI Reclaim Memory
// 4:ACPI NVS Memory
// 5:Unusable
extern char _end;

static bool is_kernel_in(uint32_t block_idx)
{
    if (
        memory_manager_g.memory_chunk[block_idx].phy_addr_start <= 0x200000 &&
        memory_manager_g.memory_chunk[block_idx].phy_addr_start + memory_manager_g.memory_chunk[block_idx].length > 0x200000) {
        return true;
    }
    return false;
}

static uint64_t get_phy_address(void *virtual_address_ptr)
{
    uint64_t virtual_address = (uint64_t)virtual_address_ptr;

    uint64_t *PML4E_ptr = (uint64_t *)PML4_ADDRESS + PML4E_IDX(virtual_address);
    ASSERT(*PML4E_ptr & 0x01);
    uint64_t PDPT_address = (uint64_t)((*PML4E_ptr) & (0xffffffffffffff00)) + HIGH_KERNEL_VIRTUAL_START;

    uint64_t *PDPTE_ptr = (uint64_t *)PDPT_address + PDPTE_IDX(virtual_address);
    ASSERT(*PDPTE_ptr & 0x01);
    uint64_t PD_address = (uint64_t)((*PDPTE_ptr) & (0xffffffffffffff00)) + HIGH_KERNEL_VIRTUAL_START;
    uint64_t *PDE_ptr = (uint64_t *)PD_address + PDE_IDX(virtual_address);
    uint64_t phy_address = *PDE_ptr & 0xffffffffffffff00;
    return phy_address;
}

void memory_init()
{
    uint64_t total_memory = 0;
    memory_manager_g.block_count = 0;

    uint64_t highest_memory = 0;
    for (uint32_t i = 0; i < bootInfo_g->memoryInfo.memoryDescriptorCount; i++) {

        struct memory_descriptor *descriptor = &(bootInfo_g->memoryInfo.memoryDescriptors[i]);
        if (descriptor->type == 1 && descriptor->length >= 1024 * 1024 * 2) {
            total_memory += descriptor->length;
            uint64_t align_2m_address = (DIV_ROUND_UP(descriptor->address, PDE_BYTE_SIZE)) * PDE_BYTE_SIZE;

            memory_manager_g.memory_chunk[memory_manager_g.block_count].phy_addr_start = align_2m_address;
            memory_manager_g.memory_chunk[memory_manager_g.block_count].length = descriptor->length + descriptor->address - align_2m_address;
            memory_manager_g.block_count++;
            if (descriptor->address + descriptor->length > highest_memory) {
                highest_memory = descriptor->address + descriptor->length;
            }
        }
        draw_printf(0, 128 + 16 * i, COLOR_WHITE, "Address:%X,Length:%X byte <=>%d mb,Type:%x",
                    descriptor->address, descriptor->length, descriptor->length / 1024 / 1024, descriptor->type);
    }
    draw_printf(0, 112, COLOR_WHITE, "Total Physical Memory->%X byte <=> %d mb", total_memory, total_memory / 1024 / 1024);
    draw_printf(0, 96, COLOR_WHITE, "Highest Physical Memory->%X byte <=> %d mb", highest_memory, highest_memory / 1024 / 1024);

    for (uint32_t i = 0; i < memory_manager_g.block_count; i++) {
        draw_printf(512, 128 + 16 * i, COLOR_WHITE, "MemoryBlock->Address:%X,Length:%X byte <=>%d mb",
                    memory_manager_g.memory_chunk[i].phy_addr_start, memory_manager_g.memory_chunk[i].length, memory_manager_g.memory_chunk[i].length / 1024 / 1024);
    }

    void *kernel_end_ptr = (void *)&_end;
    uint8_t *address_ptr = (uint8_t *)kernel_end_ptr;

    // init_bitmap
    for (uint32_t i = 0; i < memory_manager_g.block_count; i++) {
        uint64_t address = memory_manager_g.memory_chunk[i].phy_addr_start;
        uint64_t length = memory_manager_g.memory_chunk[i].length;
        struct Bitmap *bmp = &memory_manager_g.memory_chunk[i].bitmap;

        bmp->bytes_len = DIV_ROUND_UP(length / PDE_BYTE_SIZE, 8);
        bmp->last_byte_space = (length / PDE_BYTE_SIZE) % 8;
        bmp->bits = address_ptr;
        bitmap_init(bmp);
        address_ptr += bmp->bytes_len;
        if (is_kernel_in(i)) {
            for (uint32_t i = 0; i < Kernel_Reserved_Page_Count; i++) {
                bitmap_set(bmp, i, 1);
            }
        }
    }
    // init_kernel_virtual_address
    kernel_vaddr_g.vaddr_bitmap.bytes_len = DIV_ROUND_UP(total_memory / PDE_BYTE_SIZE, 8);
    kernel_vaddr_g.vaddr_bitmap.last_byte_space = (total_memory / PDE_BYTE_SIZE) % 8;
    kernel_vaddr_g.vaddr_bitmap.bits = address_ptr;
    kernel_vaddr_g.vaddr_start = K_HEAP_START;
    address_ptr += kernel_vaddr_g.vaddr_bitmap.bytes_len;

    bitmap_init(&kernel_vaddr_g.vaddr_bitmap);

    uint32_t page_2mb_count = DIV_ROUND_UP(highest_memory, PDPTE_BYTE_SIZE) + 1;
    address_ptr--;
    address_ptr = (uint8_t *)(((uint64_t)address_ptr) & 0xfffffffffffff000);
    address_ptr += PAGE_TABLE_SIZE;

    for (uint32_t i = 1; i < page_2mb_count; i++) {
        set_PDPTE(i, virtual_address_to_physical((uint64_t)address_ptr));
        memsetl(address_ptr, 0, 512);
        address_ptr += PAGE_TABLE_SIZE;
    }

    uint64_t vga_remmap_address_start = (page_2mb_count - 1) * PDPTE_BYTE_SIZE + HIGH_KERNEL_VIRTUAL_START;

    uint32_t n = DIV_ROUND_UP(bootInfo_g->graphicInfo.frameBufferSize, PDE_BYTE_SIZE);

    for (uint32_t i = 0; i < n; i++) {
        page_map_add((void *)vga_remmap_address_start + i * PDE_BYTE_SIZE, (void *)bootInfo_g->graphicInfo.frameBufferBase + i * PDE_BYTE_SIZE);
    }
    for (uint32_t i = 0; i < n; i++) {
        page_map_remove((void *)VGA_TEMP_VIRTUAL_ADDRESS + i * PDE_BYTE_SIZE, (void *)bootInfo_g->graphicInfo.frameBufferBase + i * PDE_BYTE_SIZE);
    }
    // 重新映射显存
    graphicInfo_g.frameBufferBase = vga_remmap_address_start;
    flush_tlb();
    draw_printf(0, 80, COLOR_WHITE, "Remmap VGA Success");
    block_desc_init(k_block_descs_g);
    return;
}

void set_PDPTE(uint32_t idx, uint64_t pde_address)
{
    uint64_t *ptr = (uint64_t *)PDPT_ADDRESS;
    ptr += idx;
    *ptr = (pde_address) | 0x7;
}

uint64_t virtual_address_to_physical(uint64_t virtual_address)
{
    return virtual_address - HIGH_KERNEL_VIRTUAL_START;
}

void *phy_alloc_2m()
{
    uint32_t count = memory_manager_g.block_count;
    for (uint32_t i = 0; i < count; i++) {
        int32_t idx = bitmap_scan(&memory_manager_g.memory_chunk[i].bitmap, 1);
        if (idx != -1) {
            bitmap_set(&memory_manager_g.memory_chunk[i].bitmap, idx, 1);
            return (void *)(memory_manager_g.memory_chunk[i].phy_addr_start + idx * PDE_BYTE_SIZE);
        }
    }
    return NULL;
}
void *virtual_alloc_2m()
{
    int32_t idx = bitmap_scan(&kernel_vaddr_g.vaddr_bitmap, 1);
    if (idx != -1) {
        bitmap_set(&kernel_vaddr_g.vaddr_bitmap, idx, 1);
        return (void *)(idx * PDE_BYTE_SIZE + kernel_vaddr_g.vaddr_start);
    }
    return NULL;
}

void *alloc_a_page()
{
    void *vir_ptr = virtual_alloc_2m();
    void *phy_ptr = phy_alloc_2m();
    page_map_add(vir_ptr, phy_ptr);
    return vir_ptr;
}

void free_a_page(void *virtual_address_ptr)
{
    uint64_t virtual_address = (uint64_t)virtual_address_ptr;
    uint64_t phy_address = get_phy_address(virtual_address_ptr);
    uint32_t idx = (virtual_address - kernel_vaddr_g.vaddr_start) / PDE_BYTE_SIZE;
    bitmap_set(&kernel_vaddr_g.vaddr_bitmap, idx, 0);
    uint32_t count = memory_manager_g.block_count;
    for (uint32_t i = 0; i < count; i++) {
        struct MemoryChunk *block = &memory_manager_g.memory_chunk[i];
        if (phy_address >= block->phy_addr_start && phy_address < block->phy_addr_start + block->length) {
            idx = (phy_address - block->phy_addr_start) / PDE_BYTE_SIZE;
            bitmap_set(&block->bitmap, idx, 0);
            break;
        }
    }
    page_map_remove(virtual_address_ptr, (void *)phy_address);
    return;
}

void page_map_add(void *virtual_address_ptr, void *phy_address_ptr)
{
    uint64_t virtual_address = (uint64_t)virtual_address_ptr;
    uint64_t phy_address = (uint64_t)phy_address_ptr;

    uint64_t *PML4E_ptr = (uint64_t *)PML4_ADDRESS + PML4E_IDX(virtual_address);
    ASSERT(*PML4E_ptr & 0x01);
    uint64_t PDPT_address = (uint64_t)((*PML4E_ptr) & (0xffffffffffffff00)) + HIGH_KERNEL_VIRTUAL_START;

    uint64_t *PDPTE_ptr = (uint64_t *)PDPT_address + PDPTE_IDX(virtual_address);
    ASSERT(*PDPTE_ptr & 0x01);
    uint64_t PD_address = (uint64_t)((*PDPTE_ptr) & (0xffffffffffffff00)) + HIGH_KERNEL_VIRTUAL_START;
    uint64_t *PDE_ptr = (uint64_t *)PD_address + PDE_IDX(virtual_address);
    *PDE_ptr = phy_address | 0x87;
    return;
}

void page_map_remove(void *virtual_address_ptr, void *phy_address_ptr)
{
    uint64_t virtual_address = (uint64_t)virtual_address_ptr;
    uint64_t phy_address = (uint64_t)phy_address_ptr;

    uint64_t *PML4E_ptr = (uint64_t *)PML4_ADDRESS + PML4E_IDX(virtual_address);
    ASSERT(*PML4E_ptr & 0x01);
    uint64_t PDPT_address = (uint64_t)((*PML4E_ptr) & (0xffffffffffffff00)) + HIGH_KERNEL_VIRTUAL_START;

    uint64_t *PDPTE_ptr = (uint64_t *)PDPT_address + PDPTE_IDX(virtual_address);
    ASSERT(*PDPTE_ptr & 0x01);
    uint64_t PD_address = (uint64_t)((*PDPTE_ptr) & (0xffffffffffffff00)) + HIGH_KERNEL_VIRTUAL_START;
    uint64_t *PDE_ptr = (uint64_t *)PD_address + PDE_IDX(virtual_address);
    *PDE_ptr = 0;
    return;
}

// 初始化管理不同种类型arena的不同mem_block_desc
void block_desc_init(struct MemoryBlockDescriptor *desc_array)
{
    uint32_t desc_idx, block_size = 16;
    for (desc_idx = 0; desc_idx < BLOCK_CNT; desc_idx++) {
        desc_array[desc_idx].block_size = block_size;
        desc_array[desc_idx].blocks_per_arena = (PDE_BYTE_SIZE - sizeof(struct Arena)) / block_size;
        list_init(&desc_array[desc_idx].free_list);
        block_size *= 2; // 更新为下一个规格内存块
    }
}
/* 返回arena中第idx个内存块的地址 */
static struct MemoryBlock *arena2block(struct Arena *a, uint32_t idx)
{
    return (struct MemoryBlock *)((uint32_t)a + sizeof(struct Arena) + idx * a->desc->block_size);
}

/* 返回内存块b所在的arena地址 */
static struct Arena *block2arena(struct MemoryBlock *b)
{
    return (struct Arena *)((uint32_t)b & 0xfffff000);
}
