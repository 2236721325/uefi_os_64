#include "os/kernel/init.h"
#include "os/kernel/tss.h"
#include "os/kernel/interrupt.h"
#include "os/kernel/tss.h"
#include "os/device/mouse.h"
#include "os/kernel/draw.h"
#include "os/kernel/efi_info.h"
#include "os/device/8259A.h"
#include "os/device/keyboard.h"
#include "os/kernel/memory.h"

void kernel_init()
{
    init_boot_info();
    tss_init();
    interrupt_init();
    init_draw();
    memory_init();
    keyboard_init();
    mouse_init();
}