#pragma once

#include "os/kernel/types.h"
#include <stdarg.h>


char g_buffer[4096]={0};
uint32_t sprintf(char* buf, const char* format, ...) ;
uint32_t vsprintf(char* str, const char* format, va_list ap);
void memsetl(void* dst_, uint64_t value, uint32_t size);
