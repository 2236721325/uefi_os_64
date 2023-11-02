#include "os/kernel/interrupt.h"
#include "os/kernel/draw.h"


void interrupt_init()
{
   
}

void set_idt_gate(uint8_t n,uint64_t offset,uint16_t selector,uint8_t ist,uint8_t type_attributes)
{
    void* target=(void*)IDT_Table+n*16;

    InterruptDescriptor64* p=(InterruptDescriptor64*)target;
    
    p->offset_1=(uint16_t)(offset & 0xffff);

    p->selector=selector;
    p->ist=ist;
    p->type_attributes=type_attributes;
    p->offset_2=(uint16_t)((offset>>16)& 0xffff);
    p->offset_3=(uint32_t)((offset>>32)& 0xffffffff);
}
void interrupt_handle()
{
    return ;
}

void exception_handle()
{
    return;
}

void set_tss64(unsigned long rsp0,unsigned long rsp1,unsigned long rsp2,unsigned long ist1,unsigned long ist2,unsigned long ist3,
unsigned long ist4,unsigned long ist5,unsigned long ist6,unsigned long ist7)
{
	*(unsigned long *)(TSS64_Table+1) = rsp0;
	*(unsigned long *)(TSS64_Table+3) = rsp1;
	*(unsigned long *)(TSS64_Table+5) = rsp2;

	*(unsigned long *)(TSS64_Table+9) = ist1;
	*(unsigned long *)(TSS64_Table+11) = ist2;
	*(unsigned long *)(TSS64_Table+13) = ist3;
	*(unsigned long *)(TSS64_Table+15) = ist4;
	*(unsigned long *)(TSS64_Table+17) = ist5;
	*(unsigned long *)(TSS64_Table+19) = ist6;
	*(unsigned long *)(TSS64_Table+21) = ist7;	
}

