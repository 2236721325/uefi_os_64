#include "os/kernel/interrupt.h"
#include "os/kernel/draw.h"
#include "os/device/8259A.h"
#include "os/kernel/io.h"
void *handler_table[256];
extern void *handler_entry_table[INTERRUPT_ENTRY_COUNT];

#include "os/kernel/exception.h"

void set_idt_gate(InterruptDescriptor64 *idt_gate, uint64_t offset, uint16_t selector, uint8_t ist, uint8_t type_attributes)
{
   idt_gate->offset_1 = (uint16_t)offset;
   idt_gate->selector = selector;
   idt_gate->ist = ist;
   idt_gate->type_attributes = type_attributes;
   idt_gate->offset_2 = (uint16_t)(offset >> 16);
   idt_gate->offset_3 = (uint32_t)(offset >> 32);
}

void set_interrupt_handler(uint32_t irq, void *handler)
{
   handler_table[irq] = handler;
}
void interrupt_init()
{

   for (uint32_t i = 0; i < INTERRUPT_ENTRY_COUNT; i++)
   {
      set_intr_gate(i, 1, handler_entry_table[i]);
   }
   for (uint32_t i = 0; i < 0x20; i++)
   {
      handler_table[i] = exception_handler;
   }
   for (uint32_t i = 0x20; i < INTERRUPT_ENTRY_COUNT; i++)
   {
      handler_table[i] = default_handler;
   }

   pic_init();
}

void default_handler(uint64_t rsp, uint64_t int_id)
{
   uint64_t * p = NULL;
   p = ( uint64_t *)(rsp + 0x90);
   draw_printf(0,0,COLOR_RED,"Default_Handler:int_id:%X",int_id);
   send_eoi(int_id);
}
void register_handler(uint8_t idx, void* function)
{
   handler_table[idx]=function;
}

/* 获取当前中断状态 */
enum intr_status intr_get_status()
{
   uint64_t eflags = 0;
   GET_RFLAGS(eflags);
   return (RFLAGS_IF & eflags) ? INTR_ON : INTR_OFF;
}

/* 开中断并返回开中断前的状态*/
enum intr_status intr_enable()
{
   enum intr_status old_status;
   if (INTR_ON == intr_get_status())
   {
      old_status = INTR_ON;
      return old_status;
   }
   else
   {
      old_status = INTR_OFF;
      __asm__ __volatile__("sti"); // 开中断,sti指令将IF位置1
      return old_status;
   }
}

/* 关中断,并且返回关中断前的状态 */
enum intr_status intr_disable()
{
   enum intr_status old_status;
   if (INTR_ON == intr_get_status())
   {
      old_status = INTR_ON;
      __asm__ __volatile__("cli" : : : "memory"); // 关中断,cli指令将IF位置0
                                                  // cli指令不会直接影响内存。然而，从一个更大的上下文来看，禁用中断可能会影响系统状态，
                                                  // 这个状态可能会被存储在内存中。所以改变位填 "memory" 是为了安全起见，确保编译器在生成代码时考虑到这一点。
      return old_status;
   }
   else
   {
      old_status = INTR_OFF;
      return old_status;
   }
}

/* 将中断状态设置为status */
enum intr_status intr_set_status(enum intr_status status)
{
   return status & INTR_ON ? intr_enable() : intr_disable(); // enable与disable函数会返回旧中断状态
}
