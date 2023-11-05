#pragma once
// 0x60	Read/Write	Data Port
// 0x64	Read	Status Register
// 0x64	Write	Command Registe
#include "os/kernel/types.h"
#include "os/lib/ioqueue.h"

#define KEYBUFFER_SIZE 64

ioqueue g_keyqueue;

uint8_t g_keybuf[KEYBUFFER_SIZE];



#define PS2_DATA_PORT 0x60
#define PS2_STATUS_PORT 0x64  //Read only
#define PS2_CMD_PORT 0x64      //Just Write

#define PS2_SEND_NOTREADY	0x02
#define PS2_WRITE_MODE		0x60
#define KBC_MODE			0x47

void wait_KBC_sendready(void);

void keyboard_init();