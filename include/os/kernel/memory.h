#pragma once
#include "os/kernel/global.h"
#include "os/lib/bitmap.h"
#include "os/lib/list.h"
#define PDE_BYTE_SIZE 0x200000L		  // 2mb
#define PDPTE_BYTE_SIZE 0x40000000L	  // 1GB
#define PML4E_BYTE_SIZE 0x8000000000L // 512GB
#define PAGE_TABLE_SIZE 0x1000		  // 4096
#define Kernel_Reserved_Page_Count 3
#define PML4_ADDRESS 0xffff800000101000L
#define PDPT_ADDRESS 0xffff800000102000L
#define K_HEAP_START 0xffff800001000000L
#define HIGH_KERNEL_VIRTUAL_START 0xffff800000000000L
#define PML4E_IDX(address) ((address >> 39) & 0x1FF)
#define PDPTE_IDX(address) ((address >> 30) & 0x1FF)
#define PDE_IDX(address) ((address >> 21) & 0x1FF)
// 刷新TLB使更改后的页表生效(重新赋值CR3),因为在更改页表项后,原页表项依然缓存于TLB内,重新加载页目录基地址到CR3将强制刷新TLB,达到更新页表项的目的
#define flush_tlb()               \
	do                            \
	{                             \
		unsigned long tmpreg;     \
		__asm__ __volatile__(     \
			"movq	%%cr3,	%0	\n\t" \
			"movq	%0,	%%cr3	\n\t" \
			: "=r"(tmpreg)        \
			:                     \
			: "memory");          \
	} while (0)

struct MemoryChunk
{
	struct Bitmap bitmap;
	uint64_t phy_addr_start;
	uint64_t length;
};

struct MemoryManager
{
	uint32_t block_count;
	struct MemoryChunk  memory_chunk[32];
};
struct VirtualPool
{
	struct Bitmap vaddr_bitmap; // 虚拟地址用到的位图结构
	uint64_t vaddr_start;		// 虚拟地址起始地址
};

struct MemoryBlock
{
	struct list_elem free_elem;
};
struct MemoryBlockDescriptor
{
	uint32_t block_size;
	uint32_t blocks_per_arena;
	struct list free_list;
};


struct Arena
{
	struct MemoryBlockDescriptor* desc;
	uint32_t cnt;
   	bool large;		   /* large为ture时,cnt表示的是页框数。否则cnt表示空闲mem_block数量 */
};
#define BLOCK_CNT 16
struct MemoryBlockDescriptor k_block_descs_g[BLOCK_CNT];

struct MemoryManager memory_manager_g;
struct VirtualPool kernel_vaddr_g;

void memory_init();
//初始化管理不同种类型arena的不同mem_block_desc
void block_desc_init(struct MemoryBlockDescriptor* desc_array);
void set_PDPTE(uint32_t idx, uint64_t pde_address);
uint64_t virtual_address_to_physical(uint64_t virtual_address);
void *phy_alloc_2m();
void *virtual_alloc_2m();
void *alloc_a_page();
void free_a_page(void *virtual_address);
void page_map_add(void *virtual_address, void *phy_address);
void page_map_remove(void *virtual_address_ptr, void *phy_address_ptr);

void* sys_malloc(uint32_t size);
