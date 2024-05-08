#pragma once
#include "os/kernel/types.h"

__attribute__((always_inline)) inline uint8_t io_in8(uint16_t port)
{
    uint8_t ret = 0;
    __asm__ __volatile__("inb	%%dx,	%0	\n\t"
                         "mfence			\n\t"
                         : "=a"(ret)
                         : "d"(port)
                         : "memory");
    return ret;
}

/*

*/
__attribute__((always_inline)) inline uint32_t io_in32(uint16_t port)
{
    uint32_t ret = 0;
    __asm__ __volatile__("inl	%%dx,	%0	\n\t"
                         "mfence			\n\t"
                         : "=a"(ret)
                         : "d"(port)
                         : "memory");
    return ret;
}

/*

*/
__attribute__((always_inline)) inline void io_out8(uint16_t port, uint8_t value)
{
    __asm__ __volatile__("outb	%0,	%%dx	\n\t"
                         "mfence			\n\t"
                         :
                         : "a"(value), "d"(port)
                         : "memory");
}

/*

*/
__attribute__((always_inline)) inline void io_out32(uint16_t port, uint32_t value)
{
    __asm__ __volatile__("outl	%0,	%%dx	\n\t"
                         "mfence			\n\t"
                         :
                         : "a"(value), "d"(port)
                         : "memory");
}