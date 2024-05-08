#include "os/device/keyboard.h"
#include "os/device/mouse.h"
#include "os/kernel/draw.h"
#include "os/kernel/global.h"
#include "os/kernel/init.h"

int kernel_main(void)
{
    kernel_init();
    while (1) {
    }
    // while (1)
    // {
    //     cli();
    //     if (ioqueue_empty(&g_keyqueue) && ioqueue_empty(&g_mousequeue))
    //     {
    //         sti();
    //         hlt();
    //     }
    //     else
    //     {
    //         if (!ioqueue_empty(&g_keyqueue))
    //         {
    //             uint8_t keycode = ioqueue_get(&g_keyqueue);
    //             sti();
    //             draw_solid_rect((struct Rect){0, 0, graphicInfo_g.pixelsPerScanLine, 16}, (struct Color){100, 100, 100, 0});
    //             draw_printf(0, 0, COLOR_WHITE, "KeyCode->%x", keycode);
    //         }
    //         else if (!ioqueue_empty(&g_mousequeue))
    //         {
    //             uint8_t mouse_code = ioqueue_get(&g_mousequeue);
    //             sti();

    //             if (mouse_code_g.phase == 0)
    //             {
    //                 if (mouse_code == 0xfa)
    //                 {
    //                     mouse_code_g.phase++;
    //                 }
    //             }
    //             else if (mouse_code_g.phase == 1)
    //             {
    //                 if ((mouse_code & 0xc8) == 0x08)
    //                 {
    //                     mouse_code_g.buffer[0] = mouse_code;
    //                     mouse_code_g.phase++;
    //                 }
    //             }
    //             else if (mouse_code_g.phase == 2)
    //             {
    //                 mouse_code_g.buffer[1] = mouse_code;
    //                 mouse_code_g.phase++;
    //             }
    //             else if (mouse_code_g.phase == 3)
    //             {
    //                 mouse_code_g.buffer[2] = mouse_code;
    //                 mouse_code_g.btn = mouse_code_g.buffer[0] & 0x07;
    //                 mouse_code_g.x = mouse_code_g.buffer[1];
    //                 mouse_code_g.y = mouse_code_g.buffer[2];
    //                 if ((mouse_code_g.buffer[0] & 0x10) != 0)
    //                 {
    //                     mouse_code_g.x |= 0xffffff00;
    //                 }
    //                 if ((mouse_code_g.buffer[0] & 0x20) != 0)
    //                 {
    //                     mouse_code_g.y|= 0xffffff00;
    //                 }
    //                 /* 鼠标的y方向与画面符号相反 */
    //                 mouse_code_g.y = -mouse_code_g.y;
    //                 mouse_code_g.phase = 1;

    //                 // g_mouse_x+=mouse_code_g.x;
    //                 // g_mouse_y+=mouse_code_g.y;
    //                 move_mouse((g_mouse_x+mouse_code_g.x),(g_mouse_y+mouse_code_g.y));

    //                 // draw_solid_rect((Rect){0, 16, graphicInfo_g.pixelsPerScanLine, 16}, (Color){100, 100, 100, 0});
    //                 // draw_printf(0, 16, COLOR_WHITE, "MouseCode->(%x,%x,%x)", mouse_buffer[0], mouse_buffer[1], mouse_buffer[2]);
    //             }
    //         }
    //     }
    //}
}
