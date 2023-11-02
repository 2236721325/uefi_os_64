#include "os/kernel/efi_info.h"

EfiBootInfo g_bootInfo;
void init_boot_info()
{
    EfiBootInfo* info=(EfiBootInfo*)BOOT_INFO_ADDRESS;
    g_bootInfo.graphicInfo=info->graphicInfo;
    g_bootInfo.graphicInfo.FrameBufferBase=0xffff800003000000;
}
