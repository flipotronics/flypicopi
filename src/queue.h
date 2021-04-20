/*
https://www.heise.de/developer/artikel/Des-Prozessors-Kern-Parallelisierung-auf-dem-Raspberry-Pi-Pico-mit-C-und-C-6000386.html

*/

#pragma once

#define _POSIX_THREADS 1

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "Def.h"
#include "queue.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"

const uint8_t queuesize = 50;

static MidiEvent dataQueue[queuesize];
static uint8_t pRead = 0;
static uint8_t pWrite = 0;


MidiEvent queue_read() {

    if(pRead != pWrite){
        pRead = (pRead+1) % queuesize;
        return dataQueue[pRead];
    }
    MidiEvent event;
    event.isActive = false;
    
    return event;
}

void queue_write(MidiEvent event) {
    pWrite = (pWrite+1) % queuesize;
    dataQueue[pWrite] = event;
    dataQueue[pWrite].isActive = true;
     
}


