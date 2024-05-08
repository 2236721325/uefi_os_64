#pragma once
#include "os/kernel/global.h"




struct ioqueue{
    uint8_t* buffer;			    // 缓冲区大小
    int32_t head;			        // 队首,数据往队首处写入
    int32_t tail;			        // 队尾,数据从队尾处读出
    uint32_t size;
};


void ioqueue_init(struct ioqueue* ioq,uint8_t* buf,uint32_t size);

void ioqueue_put(struct ioqueue* ioq,uint8_t value);

uint8_t ioqueue_get(struct ioqueue* ioq);


bool ioqueue_full(struct ioqueue* ioq);

bool ioqueue_empty(struct ioqueue* ioq);

