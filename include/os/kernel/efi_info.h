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
	GraphicInfo graphicInfo;
}EfiBootInfo;


extern EfiBootInfo g_bootInfo;

void init_boot_info();
