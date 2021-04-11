
#pragma once

#include <stdio.h>
#include <math.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "Def.h"

void loadPatch(uint8_t patcj){
    controls[7] = 100; // volume
    controls[74] = 100; // cutoff
}