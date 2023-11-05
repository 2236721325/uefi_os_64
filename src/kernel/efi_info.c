#include "os/kernel/efi_info.h"

EfiBootInfo g_bootInfo;
EfiBootInfo* tmp_efiInfo;
void init_boot_info()
{
    tmp_efiInfo=(EfiBootInfo*)BOOT_INFO_ADDRESS;
    g_bootInfo.graphicInfo=tmp_efiInfo->graphicInfo;
    g_bootInfo.graphicInfo.FrameBufferBase=0xffff800003000000;

}
