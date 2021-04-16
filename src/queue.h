#pragma once


#include <assert.h>
#include <stdio.h>
#include "Def.h"

const uint8_t queuesize = 50;

static MidiEvent data[queuesize];
static uint8_t pRead = 0;
static uint8_t pWrite = 0;

MidiEvent queue_read() {
    if(pRead != pWrite){
        pRead = (pRead+1) % queuesize;
        return data[pRead];
    }
    MidiEvent event;
    event.isActive = false;
    return event;
}

int queue_write(MidiEvent event) {
    pWrite = (pWrite+1) % queuesize;
    data[pWrite] = event;
    data[pWrite].isActive = true;
    return 0;
}


