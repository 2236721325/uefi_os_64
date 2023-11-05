#pragma once

#define BOOT_INFO_ADDRESS 0xffff800000060000;
typedef struct
{
	unsigned int HorizontalResolution;
	unsigned int VerticalResolution;
	unsigned int PixelsPerScanLine;
	unsigned long FrameBufferBase;
	unsigned long FrameBufferSize;
}GraphicInfo;

typedef struct 
{
	unsigned long address;
	unsigned long length;
	unsigned int  type;
}__attribute__((packed)) EFI_E820_MEMORY_DESCRIPTOR;

typedef struct 
{
	unsigned int E820_Entry_count;
	EFI_E820_MEMORY_DESCRIPTOR E820_Entry[0];
}EFI_E820_MEMORY_DESCRIPTOR_INFORMATION;


typedef struct 
{
	GraphicInfo graphicInfo;
	EFI_E820_MEMORY_DESCRIPTOR_INFORMATION memoryInfo;
}EfiBootInfo;


extern EfiBootInfo g_bootInfo;
extern EfiBootInfo*  tmp_efiInfo;


void init_boot_info();
