#pragma once

#include "os/kernel/global.h"

// Chip - Purpose	I/O port
// Master PIC - Command	0x0020
// Master PIC - Data	0x0021
// Slave PIC - Command	0x00A0
// Slave PIC - Data	0x00A1

#define PIC_MASTER_COMMAND 0x20
#define PIC_MASTER_DATA 0x21
#define PIC_SLAVE_COMMAND 0xa0
#define PIC_SLAVE_DATA 0xa1

#define PIC_EOI		0x20	

void send_eoi(uint64_t idx);



void pic_init();
