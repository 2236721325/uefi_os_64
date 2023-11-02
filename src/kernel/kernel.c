#include "os/kernel/efi_info.h"
#include "os/kernel/draw.h"
#include "os/device/mouse.h"
#include "os/kernel/interrupt.h"
#include "os/kernel/trap.h"
static void init_all();
static void test_all();
int kernel_main(void)
{
    init_all();
    


    int c=1/0;
    while (1)
    {
        /* code */
    }
    

}

void init_all()
{
    init_boot_info();
    // load_TR(8);
	set_tss64(0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00);
    sys_trap_init();
    init_draw();
    init_mouse();
}
void test_all()
{
    draw_test();
}

