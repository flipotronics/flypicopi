#pragma once


#include <assert.h>
#include <stdio.h>
#include "Def.h"


typedef struct {
	    size_t head;
	    size_t tail;
	    size_t size = 50;
	    MidiEvent** data;
	} queue_t;

static queue_t * queue = new queue_t();
	
MidiEvent * queue_read(queue_t *queue) {
    if (queue->tail == queue->head) {
        return NULL;
    }
    void* handle = queue->data[queue->tail];
    queue->data[queue->tail] = NULL;
    queue->tail = (queue->tail + 1) % queue->size;
    return (MidiEvent *)handle;
}

int queue_write(queue_t *queue, MidiEvent * handle) {
    if (((queue->head + 1) % queue->size) == queue->tail) {
        return -1;
    }
    queue->data[queue->head] = handle;
    queue->head = (queue->head + 1) % queue->size;
    return 0;
}


