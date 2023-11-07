#include "os/kernel/debug.h"
#include "os/kernel/draw.h"
#include "os/kernel/interrupt.h"

/* 打印文件名,行号,函数名,条件并使程序悬停 */
void panic_spin(char* filename, int line, const char* func, const char* condition) 
{
   intr_disable();	//发生错误时打印错误信息，不应该被打扰
   draw_printf(0,800,COLOR_RED,"!!!!! error !!!!!");
   draw_printf(0,816,COLOR_RED,"filename:%s",filename);
   draw_printf(0,832,COLOR_RED,"line:%D",line);
   draw_printf(0,848,COLOR_RED,"function:%s",func);
   draw_printf(0,864,COLOR_RED,"condition:%s",condition);
   while(1);
}

