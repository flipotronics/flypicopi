// author: mat@flipotronics.com

#ifndef Def_h
#define Def_h

// Types
#define SAMPLE int32_t

// Math
#define M_PI  3.14159265358979323846


// Synth
#define VERSION "1.0.0"
#define BUILDDATE "3rd. March 2021"
#define MAXVOICES 8
#define MAXZONES 1

#define SINE_WAVE_TABLE_LEN 2048
#define SAMPLES_PER_BUFFER 256
#define SAMLERATE 44100

static int sine_wave_table[SINE_WAVE_TABLE_LEN];
static int saw_wave_table[SINE_WAVE_TABLE_LEN];

static uint8_t  lastParam;
static uint8_t  lastValue;

static uint8_t controls[128];
static int lastShown = -1;

struct MidiEvent{
    uint8_t b0;
    uint8_t b1;
    uint8_t b2;
};

#endif /* Def_h */