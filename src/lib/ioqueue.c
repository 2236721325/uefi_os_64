#include "os/lib/ioqueue.h"



void ioqueue_init(struct ioqueue* ioq,uint8_t* buf,uint32_t size)
{
    ioq->buffer=buf;
    ioq->size=size;
    ioq->head=0;
    ioq->tail=0;
}


void ioqueue_put(struct ioqueue* ioq,uint8_t value)
{
   ioq->buffer[ioq->head] = value;      // 把字节放入缓冲区中
   ioq->head = (ioq->head+1)%ioq->size;
}

uint8_t ioqueue_get(struct ioqueue* ioq)
{
    uint8_t byte=ioq->buffer[ioq->tail];
    ioq->tail = (ioq->tail+1)%ioq->size;
    return byte;
}


bool ioqueue_full(struct ioqueue* ioq)
{
    return (ioq->head+1)%ioq->size == ioq->tail;
}

bool ioqueue_empty(struct ioqueue* ioq)
{
    return ioq->head==ioq->tail;
}