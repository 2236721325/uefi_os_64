#include "os/device/keyboard.h"
#include "os/kernel/interrupt.h"
#include "os/kernel/global.h"
#include "os/kernel/draw.h"
#include "os/kernel/io.h"
#include "os/device/8259A.h"
static void keyboard_handler(uint64_t rsp,uint64_t int_id)
{
    uint64_t * p = NULL;
    p = ( uint64_t *)(rsp + 0x90);
    draw_printf(0,32,COLOR_RED,"KeyBoard:int_id------->%x",int_id);

    send_eoi(int_id);
    return;
}
void keyboard_init()
{
    register_handler(0x21,keyboard_handler);
}


