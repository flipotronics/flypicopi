//
//  Engine.h
//  TestLib
//
//  author: mat@flipotronics.com
//

#ifndef Engine_hpp
#define Engine_hpp

#include <stdio.h>
#include <string.h>
#include <ostream>
#include <iostream>

#include "Def.h"
#include "Renderer.h"

// =================================================== DEFINE ====================================================================
#define I2C_DATAPIN 9
#define I2C_CLOCKPIN 10

// =================================================== Enums Structs====================================================================
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

enum VoiceAllocAlgo{
    OLDEST,
    YOUNGEST,
    LOWEST,
    HIGHEST,
    IGNORE
};

struct V{
    uint8_t vid = 0;
    bool isUsed = false;
    bool isPlaying = false;
    uint32_t phase = 0;
    uint32_t step ;
    uint8_t velocity = 64;
    uint8_t midiNote = 0;
};

struct audio_buffer_pool *init_audio() {
    static audio_format_t audio_format = {
            .sample_freq = SAMLERATE, 
            .format = AUDIO_BUFFER_FORMAT_PCM_S16,
            .channel_count = 2
    };

    static struct audio_buffer_format producer_format = {
            .format = &audio_format,
            .sample_stride = 4
    };

    struct audio_buffer_pool *producer_pool = audio_new_producer_pool(&producer_format, 6,SAMPLES_PER_BUFFER); // todo correct size
    
    struct audio_i2s_config config = {
            .data_pin = I2C_DATAPIN, //9
            .clock_pin_base = I2C_CLOCKPIN,//10
            .dma_channel = 1,
            .pio_sm = 0,
    };

    const struct audio_format *output_format;
    output_format = audio_i2s_setup(&audio_format, &config);
    if (!output_format) {
        printf("Audio setup failed");
        panic("PicoAudio: Unable to open audio device.\n");
    }

    bool ok = audio_i2s_connect(producer_pool);
    assert(ok);
    audio_i2s_set_enabled(true);
    return producer_pool;
}

// =================================================== Member ====================================================================

bool __unused ok;

VoiceMode voiceMode = POLY;
PlayMode voiceMplayModeode = RETRIGGER;
VoiceAllocAlgo voiceAllocAlgo = IGNORE;

const uint32_t pos_max = 0x10000 * SINE_WAVE_TABLE_LEN;
const int multi = 1 << 16;

uint8_t noOfVoicesTotal;
uint8_t volume;

// this is the main Voices Table
V voices[MAXVOICES];

float tuning = 440.0;

// Audio Buffer
struct audio_buffer_pool *ap = init_audio();

// =================================================== Methods ====================================================================
uint32_t calcStep(float freq){
    int multi = 1 << 16;
    return ( multi * freq * (float)SINE_WAVE_TABLE_LEN / (float)SAMLERATE);
}

float freqFromNoteNumber(uint8_t number, float tuning){
    float d = (number-69) / 12.0;
    return pow(2, d) * tuning;
}

void setupWavetable(){
    for (int i = 0; i < SINE_WAVE_TABLE_LEN; i++) {
        sine_wave_table[i] = 32767.0 * cosf(i * 2.0 * (float) (M_PI / SINE_WAVE_TABLE_LEN));
        saw_wave_table[i] = -32767.0 + i * 32767 / SINE_WAVE_TABLE_LEN;
    }
}

void renderAudio(){
    while(true){
        struct audio_buffer *buffer = take_audio_buffer(ap, true);
        int32_t *samples = (int32_t *) buffer->buffer->bytes;

        // AUDIO LOOP
        for (uint i = 0; i < buffer->max_sample_count ; ++i) {
            samples[i] = 0;//2048 >> 8u; 

            for(uint y=0; y < MAXVOICES; ++y){
                if(voices[y].isPlaying){
                    samples[i] += (((int)sine_wave_table[voices[y].phase >> 16u]) >>3); 

                    //samples[i] = (vol * sine_wave_table[pos >> 16u]) >> 8u;
                    // int r = ( ((int)saw_wave_table[voices[y].phase >> 16u]))  >> 9u;
                    // samples[i] += r;

                    voices[y].phase += voices[y].step;
                    if (voices[y].phase >= pos_max) voices[y].phase -= pos_max;
                }
            }
        }
        buffer->sample_count = buffer->max_sample_count;
        give_audio_buffer(ap, buffer);
    }
}

uint8_t  findVoice( uint8_t midiNote){
    for(int y=0;y < MAXVOICES;y++){
        if(voices[y].midiNote ==  midiNote){
            return y;
        }
    }
    for(int y=0; y < MAXVOICES; y++){
        if(!voices[y].isUsed){
            return y;
        }
    }
    return 0;// make better - different algos
}  

void noteOn(uint8_t midiNote, uint8_t velocity){
    uint8_t vid = findVoice(midiNote);
    float freq = freqFromNoteNumber(midiNote, 440.0);
    voices[vid].step = calcStep(freq);
    voices[vid].isPlaying = true;
    voices[vid].isUsed = true;
    voices[vid].phase = 0;
    voices[vid].velocity = velocity;
    voices[vid].midiNote = midiNote;
    printf("note On %i %i %i  \n",vid, midiNote, velocity);
}

void noteOff(uint8_t midiNote){
    uint8_t vid = findVoice(midiNote);
    voices[vid].isPlaying = false;
    voices[vid].isUsed = false;
    printf("note Off %i  %i  \n", vid , midiNote);
}

void setTuning(uint8_t  freq){
    tuning = freq;
}

void initVoices(){
    for(uint8_t y=0;y < MAXVOICES;++y){
        voices[y].vid = y;
        voices[y].isPlaying = false;
        voices[y].phase = 0;
        voices[y].step ;
        voices[y].velocity = 127;
        voices[y].midiNote = 0;
    }
}

void aftertouch( uint8_t pressure){
  //  std::cout << "Note Aftertouch channel: " << channel << " pressure: " << pressure  << std::endl;
}

void allNotesOff(){
    for(uint8_t i=0;i > MAXVOICES; ++i){
        voices[i].isPlaying = false;
        voices[i].isUsed = false;
    }
}
    
void printTime(){
    // std::cout << "Render Time " << ms_renderTime << std::endl;
}

#endif 