#include "os/device/keyboard.h"
#include "os/kernel/interrupt.h"
#include "os/kernel/global.h"
#include "os/kernel/draw.h"
#include "os/kernel/io.h"
#include "os/device/8259A.h"

static void keyqueue_init()
{
    ioqueue_init(&g_keyqueue,g_keybuf,KEYBUFFER_SIZE);
}


static void keyboard_handler(uint64_t rsp,uint64_t int_id)
{

    uint8_t key_code=io_in8(PS2_DATA_PORT);
    if(!ioqueue_full(&g_keyqueue))
    {
        ioqueue_put(&g_keyqueue,key_code);

    }
    send_eoi(int_id);
    return;
}
void keyboard_init()
{
    keyqueue_init();
    register_handler(0x21,keyboard_handler);
    wait_KBC_sendready();
	io_out8(PS2_CMD_PORT, PS2_WRITE_MODE);
	wait_KBC_sendready();
	io_out8(PS2_DATA_PORT, KBC_MODE);
	return;
}


void wait_KBC_sendready(void) {
	/* 等待键盘控制电路准备完毕 */
	for (;;) {
		if ((io_in8(PS2_STATUS_PORT) & PS2_SEND_NOTREADY) == 0) {
			break;
		}
	}
	return;
}