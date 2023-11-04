#pragma once
#include "os/kernel/global.h"


#define INTERRUPT_ENTRY_COUNT 0x30
typedef struct 
{
   uint16_t offset_1;        // offset bits 0..15
   uint16_t selector;        // a code segment selector in GDT or LDT
   uint8_t  ist;             // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
   uint8_t  type_attributes; // gate type, dpl, and p fields
   uint16_t offset_2;        // offset bits 16..31
   uint32_t offset_3;        // offset bits 32..63
   uint32_t zero;            // reserved
}InterruptDescriptor64;

extern InterruptDescriptor64 IDT_Table[256];

void set_idt_gate(InterruptDescriptor64* ptr,uint64_t offset,uint16_t selector,uint8_t ist,uint8_t type_attributes);


void interrupt_init();



void default_handler(uint64_t rsp,uint64_t int_id);



/* 定义中断的两种状态:
 * INTR_OFF值为0,表示关中断,
 * INTR_ON值为1,表示开中断 */
enum intr_status {		 // 中断状态
    INTR_OFF,			 // 中断关闭
    INTR_ON		         // 中断打开
};

enum intr_status intr_get_status(void);
enum intr_status intr_set_status (enum intr_status);
enum intr_status intr_enable (void);
enum intr_status intr_disable (void);

__attribute__((always_inline)) 
inline void set_intr_gate(uint32_t n,uint8_t ist,void * addr)
{
   set_idt_gate(IDT_Table+n,(uint64_t)addr,0x08,ist,0x8E );	//P,DPL=0,TYPE=E
}
__attribute__((always_inline)) 
inline void set_trap_gate( uint32_t n,uint8_t ist,void * addr)
{
      set_idt_gate(IDT_Table+n,(uint64_t)addr,0x08,ist,0x8F );//P,DPL=0,TYPE=F
}
__attribute__((always_inline)) 
inline void set_system_gate( uint32_t n,uint8_t ist,void * addr)
{
   set_idt_gate(IDT_Table+n,(uint64_t)addr,0x08,ist,0xEF );//P,DPL=3,TYPE=F

}
__attribute__((always_inline)) 
inline void set_system_intr_gate(uint32_t n,uint8_t ist,void * addr)	//int3
{
   set_idt_gate(IDT_Table+n,(uint64_t)addr,0x08,ist,0xEE );//P,DPL=3,TYPE=E
}


void register_handler(uint8_t idx, void* function);
