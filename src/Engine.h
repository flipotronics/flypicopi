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
#include "time.h"

#include "Def.h"
#include "queue.h"
#include "lfo.h"

// =================================================== DEFINE ====================================================================
#define I2C_DATAPIN 9
#define I2C_CLOCKPIN 10

using namespace std;

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
    absolute_time_t tStart;
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

void setupDisplay() {
	int h = 64;
	init_display(h);
    const char * welcome = "Fly Pico Pi\nMicro Synthesizer\nFliptronics V1.0\nMade with Love\nFrankurt/M\n1 Another line\n2 Another line\n3 Another line";
	ssd1306_print(welcome,1); // demonstrate some text
	show_scr();
	//sleep_ms(2000);
	//fill_scr(0); // empty the screen

	//drawBitmap(0, 0, splash1_data, 128, 64, 1);
	//show_scr();
/*
	sleep_ms(2000);
	fill_scr(0);
	setCursorx(0);
	setCursory(0);
	ssd1306_print("Testing cursor");
	show_scr();
	sleep_ms(2000);
	setCursorx(0);
	ssd1306_print("Overwritten   ");
	show_scr();
    */
}

void setupThread2(){
    setupDisplay();
}

void printDisplay(string text){
	setCursorx(0);
    setCursory(0);
    fill_scr(0);
    ssd1306_print(text.c_str(), 1); 
	show_scr();
}

uint8_t findVoice( uint8_t midiNote){
    for(int y=0;y < MAXVOICES; ++y){
        if(voices[y].midiNote ==  midiNote){
            return y;
        }
    }
    for(int y=0; y < MAXVOICES; ++y){
        if(!voices[y].isUsed){
            return y;
        }
    }
    // find oldest voice
    uint8_t oldestId = 0;
    absolute_time_t last = voices[0].tStart;
    for(int y=1;y < MAXVOICES; ++y){
        if(voices[y].tStart < last){
            last = voices[y].tStart;
            oldestId = y;
        }
    }
    return oldestId; // 
}  
void i2c_writeDac2(uint16_t val) {
    #if DEBUG_SHOW_MIDI
    printf ("Sending adsr %i \n" ,val);
    #endif
    buf[0] = MCP4725_CMD_WRITEDAC;
    buf[1] = val / 16;
    buf[2] = (val % 16) << 4 ;
    i2c_write_blocking(PMP_DAC_DEVICE, MCP4725_I2CADDR_2, buf, 3, false);
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
    voices[vid].tStart = get_absolute_time();
    lfo_noteOn();
    #if DEBUG_SHOW_MIDI
    printf("note On %i %i %i  \n",vid, midiNote, velocity);
    #endif 
}

void noteOff(uint8_t midiNote){
    for(int y=0;y < MAXVOICES; ++y){
        if(voices[y].midiNote ==  midiNote){
          voices[y].isPlaying = false;
          voices[y].isUsed = false;
          return;
        }
    }
}

void control(uint8_t cc, uint8_t value){
    midiLightCounter = 100;
     #if DEBUG_SHOW_MIDI
    printf("cc: %i value: %i", cc, value);
    #endif

    controls[cc] = value;
    
    // Cutoff Freq
    if(cc == 74){
        i2c_writeDac2(mpc_voltages[value]); 
    }
    lastShown = cc;
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


inline static int MIDIByteInRange(uint8_t v, uint8_t min, uint8_t max) {
    return (v >= min) && (v < max);
};

void renderAudio(){
    setupThread2();
    while(true){

        //absolute_time_t tStart = get_absolute_time();

        // Midi Parser
        while(true){
            MidiEvent  ev = queue_read();
            if(ev.isActive ){
                ev.isActive = false;
               // printf("Event %i %i %i \n", ev.b0, ev.b1 , ev.b2);
                if (ev.type == NOTEON) {
                    uint8_t vid = findVoice(ev.b1);
                    float freq = freqFromNoteNumber(ev.b1, 440.0);
                    voices[vid].step = calcStep(freq);
                    voices[vid].isPlaying = true;
                    voices[vid].isUsed = true;
                    voices[vid].phase = 0;
                    voices[vid].velocity = ev.b2;
                    voices[vid].midiNote = ev.b1;
                    voices[vid].tStart = get_absolute_time();
                    lfo_noteOn();
                    #if DEBUG_SHOW_MIDI
                    printf("note On %i %i %i  \n",vid, midiNote, velocity);
                    #endif 
                    continue;
                }
                if (ev.type == NOTEOFF) {
                    for(int y=0;y < MAXVOICES; ++y){
                        if(voices[y].midiNote ==  ev.b1){
                        voices[y].isPlaying = false;
                        voices[y].isUsed = false;
                        continue;
                        }
                    }
                    continue;
                }
               if (ev.type == CONTROL) {
                    control(ev.b1,ev.b2);
                    continue;
                }
            }else{
                break;
            }
        }

        struct audio_buffer *buffer = take_audio_buffer(ap, true);
        int32_t *samples = (int32_t *) buffer->buffer->bytes;

        // AUDIO LOOP
        uint bufSize = buffer->max_sample_count;
       
        for (uint i = 0; i < buffer->max_sample_count ; ++i) {
            samples[i] = 0;//2048 >> 8u; 

            for(uint y=0; y < MAXVOICES; ++y){
                if(voices[y].isPlaying){
                    samples[i] += (controls[7] * ((int)sine_wave_table[voices[y].phase >> 16u]) >> 10); 
                    int r = ( controls[7] *((int)saw_wave_table[voices[y].phase >> 16u]))  >> 10u;
                    samples[i] += r;

                    voices[y].phase += voices[y].step;
                    if (voices[y].phase >= pos_max) voices[y].phase -= pos_max;
                }
            }
        }
     
        buffer->sample_count = bufSize;
        give_audio_buffer(ap, buffer);


       // calculate time
       // absolute_time_t tEnd = get_absolute_time();
       //taken = tEnd - tStart;
       
    }
}
#endif 