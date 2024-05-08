#pragma once

#define BOOT_INFO_ADDRESS 0xffff800000060000;

struct graphic_info {
    unsigned int horizontalResolution;
    unsigned int verticalResolution;
    unsigned int pixelsPerScanLine;
    unsigned long frameBufferBase;
    unsigned long frameBufferSize;
};

struct memory_descriptor {
    unsigned long address;
    unsigned long length;
    unsigned int type;
} __attribute__((packed));

struct memory_info {
    unsigned int memoryDescriptorCount;
    struct memory_descriptor memoryDescriptors[0];
};

struct boot_info {
    struct graphic_info graphicInfo;
    struct memory_info memoryInfo;
};

struct boot_info *bootInfo_g;

void boot_info_init();
