
#include <stdio.h>
#include <math.h>

#include "Voice.h"
#include "Def.h"

enum VoiceMode{
    POLY,
    MONO,
    UNISONO,
    TWOVOICES
};

enum PlayMode{
    RETRIGGER,
    LEGSTO,
    GLISSANDO
};

enum Algo{
    OLDEST,
    YOUNGEST,
    LOWEST,
    HIGHEST,
    IGNORE
};

struct Voice  voices[MAXVOICES];

void render(SAMPLE * l, SAMPLE * r, int noOfSamples){

}