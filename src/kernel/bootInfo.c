#include "os/kernel/boot_info.h"

void boot_info_init()
{
    bootInfo_g = (struct boot_info *)BOOT_INFO_ADDRESS;
}
