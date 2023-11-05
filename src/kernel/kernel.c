#include "os/kernel/init.h"
#include "os/kernel/global.h"
#include "os/device/keyboard.h"
#include "os/device/mouse.h"
#include "os/kernel/draw.h"

int kernel_main(void)
{
    kernel_init();
    while (1)
    {
        cli();
        if(ioqueue_empty(&g_keyqueue)&&ioqueue_empty(&g_mousequeue))
        {
            sti();
            hlt();
        }
        else 
        {
            if(!ioqueue_empty(&g_keyqueue))
            {
                uint8_t keycode=ioqueue_get(&g_keyqueue);
                sti();
                draw_solid_rect((Rect){0,0,g_graphicInfo->PixelsPerScanLine,16},(Color){100,100,100,0});
                draw_printf(0,0,COLOR_WHITE,"KeyCode->%x",keycode);

            }
            else if(!ioqueue_empty(&g_mousequeue))
            {
                uint8_t mouse_code=ioqueue_get(&g_mousequeue);
                sti();
                draw_solid_rect((Rect){0,16,g_graphicInfo->PixelsPerScanLine,16},(Color){100,100,100,0});
                draw_printf(0,16,COLOR_WHITE,"MouseCode->%x",mouse_code);
            }
        }

    }
    

}



