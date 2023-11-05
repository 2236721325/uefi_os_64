#include "os/device/8259A.h"
#include "os/kernel/io.h"
#include "os/kernel/global.h"
#include "os/kernel/interrupt.h"
#include "os/kernel/io.h"
void pic_init()
{

    intr_disable();

    // 8259A-master     ICW1-4
    io_out8(PIC_MASTER_COMMAND, 0x11); // ICW1: 边沿触发,级联8259, 需要ICW4.
    io_out8(PIC_MASTER_DATA, 0x20);    // ICW2: 起始中断向量号为0x20,也就是IR[0-7] 为 0x20 ~ 0x27.
    io_out8(PIC_MASTER_DATA, 0x04);    // ICW3: IR2接从片.
    io_out8(PIC_MASTER_DATA, 0x01);    // ICW4: 8086模式, 正常EOI

    // 8259A-slave     ICW1-4
    io_out8(PIC_SLAVE_COMMAND, 0x11); // ICW1: 边沿触发,级联8259, 需要ICW4.
    io_out8(PIC_SLAVE_DATA, 0x28);    // ICW2: 起始中断向量号为0x28,也就是IR[8-15] 为 0x28 ~ 0x2F.
    io_out8(PIC_SLAVE_DATA, 0x02);    // ICW3: 设置从片连接到主片的IR2引脚
    io_out8(PIC_SLAVE_DATA, 0x01);    // ICW4: 8086模式, 正常EOI

    // 8259A-M/S     OCW1
    io_out8(PIC_MASTER_DATA, 0xf9);
    io_out8(PIC_SLAVE_DATA, 0xef);

    intr_enable();
}

void send_eoi(uint64_t idx)
{
    if (idx - 0x20 >= 8)
    {
        io_out8(PIC_SLAVE_COMMAND, PIC_EOI); // 在中断处理之后向8259A发送EOI通知它中断处理结束
    }
    io_out8(PIC_MASTER_COMMAND, PIC_EOI); // 在中断处理之后向8259A发送EOI通知它中断处理结束
}
