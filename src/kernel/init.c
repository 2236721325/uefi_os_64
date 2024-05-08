#include "os/kernel/init.h"
#include "os/device/8259A.h"
#include "os/device/keyboard.h"
#include "os/device/mouse.h"
#include "os/kernel/boot_info.h"
#include "os/kernel/draw.h"
#include "os/kernel/interrupt.h"
#include "os/kernel/memory.h"
#include "os/kernel/tss.h"

void kernel_init()
{
    boot_info_init();
    draw_init();
    tss_init();
    interrupt_init();
    memory_init();
    // keyboard_init();
    // mouse_init();
    // for (int i = 0; i < 100; i++) {
    //     void *test = alloc_a_page();
    //     uint32_t *hello = (uint32_t *)test;
    //     hello++;
    //     *hello = 10;
    //     free_a_page(test);
    // }
}