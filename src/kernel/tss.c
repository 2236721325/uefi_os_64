#include "os/kernel/tss.h"

void tss_init()
{
    set_tss64(0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00);
}

void set_tss64(uint64_t rsp0,uint64_t rsp1,uint64_t rsp2,uint64_t ist1,uint64_t ist2,uint64_t ist3,
uint64_t ist4,uint64_t ist5,uint64_t ist6,uint64_t ist7)
{
	*(uint64_t *)(TSS64_Table+1) = rsp0;
	*(uint64_t *)(TSS64_Table+3) = rsp1;
	*(uint64_t *)(TSS64_Table+5) = rsp2;
	*(uint64_t *)(TSS64_Table+9) = ist1;
	*(uint64_t *)(TSS64_Table+11) = ist2;
	*(uint64_t *)(TSS64_Table+13) = ist3;
	*(uint64_t *)(TSS64_Table+15) = ist4;
	*(uint64_t *)(TSS64_Table+17) = ist5;
	*(uint64_t *)(TSS64_Table+19) = ist6;
	*(uint64_t *)(TSS64_Table+21) = ist7;
}