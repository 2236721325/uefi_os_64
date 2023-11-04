#include "os/device/mouse.h"
#include "os/kernel/efi_info.h"
#include "os/kernel/types.h"
#include "os/kernel/draw.h"
#include "os/kernel/interrupt.h"
#include "os/kernel/io.h"
#include "os/device/8259A.h"
static void mouse_handler(uint64_t rsp,uint64_t int_id)
{

	uint64_t * p = NULL;
    p = ( uint64_t *)(rsp + 0x90);
    draw_printf(0,64,COLOR_RED,"Mouse:Error_code------->%x",int_id);

    send_eoi(int_id);
	return ;
}
void mouse_init()
{
	register_handler(0x2c,mouse_handler);
    g_mouse_x=g_graphicInfo->HorizontalResolution/2;
    g_mouse_y= g_graphicInfo->VerticalResolution/2;
    draw_mouse();
}


void draw_mouse()
{
	static char cursor[16][16] = {
		"**************..",
		"*OOOOOOOOOOO*...",
		"*OOOOOOOOOO*....",
		"*OOOOOOOOO*.....",
		"*OOOOOOOO*......",
		"*OOOOOOO*.......",
		"*OOOOOOO*.......",
		"*OOOOOOOO*......",
		"*OOOO**OOO*.....",
		"*OOO*..*OOO*....",
		"*OO*....*OOO*...",
		"*O*......*OOO*..",
		"**........*OOO*.",
		"*..........*OOO*",
		"............*OO*",
		".............***"
	};
	int x, y;

	for (y = 0; y < 16; y++) {
		for (x = 0; x < 16; x++) {
			if (cursor[y][x] == '*') {
                draw_pixel(g_mouse_x+x,g_mouse_y+y,(Color){0,0,0,0});
			}
			if (cursor[y][x] == 'O') {
                draw_pixel(g_mouse_x+x,g_mouse_y+y,(Color){255,255,255,0});
			}
			if (cursor[y][x] == '.') {
                draw_pixel(g_mouse_x+x,g_mouse_y+y,(Color){100,100,100,0});
			}
		}
	}
	return;
}
