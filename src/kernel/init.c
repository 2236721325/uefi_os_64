#include "os/kernel/init.h"
#include "os/kernel/tss.h"
#include "os/kernel/interrupt.h"
#include "os/kernel/tss.h"
#include "os/device/mouse.h"
#include "os/kernel/draw.h"
#include "os/kernel/efi_info.h"
#include "os/device/8259A.h"
#include "os/device/keyboard.h"

void kernel_init()
{
    init_boot_info();
    tss_init();
    interrupt_init();
    init_draw();
    mouse_init();
    keyboard_init();
    
}