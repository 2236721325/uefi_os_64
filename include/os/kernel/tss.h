#pragma once
#include "os/kernel/types.h"
void tss_init();

extern uint32_t TSS64_Table[26];

void set_tss64(uint64_t rsp0,uint64_t rsp1,uint64_t rsp2,uint64_t ist1,uint64_t ist2,uint64_t ist3,
uint64_t ist4,uint64_t ist5,uint64_t ist6,uint64_t ist7);
void tss_init();