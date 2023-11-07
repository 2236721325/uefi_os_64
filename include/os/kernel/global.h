#pragma once
#include "os/kernel/types.h"



#define RFLAGS_IF   0x00000200       // eflags寄存器中的if位为1
#define GET_RFLAGS(EFLAG_VAR) __asm__ __volatile__("pushfq; popq %0" : "=g" (EFLAG_VAR))


#define DIV_ROUND_UP(X, STEP) ((X + STEP - 1) / (STEP))  //用于向上取整的宏，如9/10=1


#define sti() 		__asm__ __volatile__ ("sti	\n\t":::"memory")
#define cli()	 	__asm__ __volatile__ ("cli	\n\t":::"memory")
#define nop() 		__asm__ __volatile__ ("nop	\n\t")
#define hlt()       __asm__ __volatile__ ("hlt	\n\t")
#define io_mfence() 	__asm__ __volatile__ ("mfence	\n\t":::"memory")



