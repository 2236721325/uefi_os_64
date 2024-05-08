#include "os/device/mouse.h"
#include "os/kernel/bootInfo.h"
#include "os/kernel/types.h"
#include "os/kernel/draw.h"
#include "os/kernel/interrupt.h"
#include "os/kernel/io.h"
#include "os/device/8259A.h"
#include "os/device/keyboard.h"

static void mouse_handler(uint64_t rsp, uint64_t int_id)
{

	uint8_t data = io_in8(PS2_DATA_PORT);
	if (!ioqueue_full(&g_mousequeue))
	{
		ioqueue_put(&g_mousequeue, data);
	}
	send_eoi(int_id);
	return;
}

#define KEYCMD_SENDTO_MOUSE 0xd4
#define MOUSECMD_ENABLE 0xf4
static void enable_mouse(void)
{
	/* 激活鼠标 */
	wait_KBC_sendready();
	io_out8(PS2_CMD_PORT, KEYCMD_SENDTO_MOUSE);
	wait_KBC_sendready();
	io_out8(PS2_DATA_PORT, MOUSECMD_ENABLE);
	return;
}
void mouse_init()
{
	ioqueue_init(&g_mousequeue, g_mousebuf, MOUSEBUFFER_SIZE);
	register_handler(0x2c, mouse_handler);
	enable_mouse();
	g_mouse_x = graphicInfo_g.horizontalResolution / 2;
	g_mouse_y = graphicInfo_g.verticalResolution / 2;
	draw_mouse();
}

void move_mouse(int x, int y)
{
	draw_solid_rect((struct Rect){g_mouse_x, g_mouse_y, 16, 16}, (struct Color){100, 100, 100, 0});
	g_mouse_x=x;
	g_mouse_y=y;
	if (g_mouse_x < 0)
	{
		g_mouse_x = 0;
	}
	if (g_mouse_y < 0)
	{
		g_mouse_y = 0;
	}
	if (g_mouse_y > graphicInfo_g.verticalResolution - 16)
	{
		g_mouse_y = graphicInfo_g.verticalResolution - 16;
	}
	if (g_mouse_x > graphicInfo_g.horizontalResolution - 16)
	{
		g_mouse_x = graphicInfo_g.horizontalResolution - 16;
	}
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
		".............***"};
	int x, y;

	for (y = 0; y < 16; y++)
	{
		for (x = 0; x < 16; x++)
		{
			if (cursor[y][x] == '*')
			{
				draw_pixel(g_mouse_x + x, g_mouse_y + y, (struct Color){0, 0, 0, 0});
			}
			if (cursor[y][x] == 'O')
			{
				draw_pixel(g_mouse_x + x, g_mouse_y + y, (struct Color){255, 255, 255, 0});
			}
			if (cursor[y][x] == '.')
			{
				draw_pixel(g_mouse_x + x, g_mouse_y + y, (struct Color){100, 100, 100, 0});
			}
		}
	}
	return;
}
