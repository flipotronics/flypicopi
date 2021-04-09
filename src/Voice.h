//
//  Voice.hpp
//  TestLib
//
//  author: mat@flipotronics.com
//

#ifndef Voice_hpp
#define Voice_hpp

#include <stdio.h>

#include "Def.h"
#include "Renderer.h"

class Voice : Renderer{
public:
    bool active = false;
    bool isOn;

    uint32_t id;
    uint32_t zone;
    uint32_t midiNote;
    uint32_t maxBend = 1;
    uint32_t minBend = 1;

    float volume = 1.0;
    float pitchbender = 1.0;
    float freq;
    float step;
    float tuning = 440.0;
    float pos;

    uint32_t pos_max =  2048;

    void pitchBend(int val){
        if(midiNote ==0) return;
        if(val > 64){
            pitchbender = 1;
        }
        if(val > 64){
            int b = (val - 64) / 64.0;
            pitchbender = 1.0 + b *  int(maxBend) / 12.0  ;
        }
        if(val < 64){
            int b = (64 - val) / 64.0;
            pitchbender = 1.0 -  b *  int(minBend) / 12.0;
        }

        freq = tuning * pow(2.0, (midiNote - 69)/12.0) * pitchbender;
       // std::cout << " freq " << freq << std::endl;
        step = freq;
    }
    
    void modWheel(int val){
        //std::cout << " mod " << val << std::endl;
    }
    
    void controller(int cc, int val){
        switch(cc){
            case 7:
                volume = int(val) / 127.0;
                break;
                
            default:
                break;
        }
    }
    
    void init(){

    }

    float freqFromNoteNumber(uint8_t number, float tuning){
        float d = (number-69) / 12.0;
        return pow(2, d) * tuning;
    }

    float calcStep(float freq){
        //float multi = 1 << 16;
        return 0;//freq * (float)SINE_WAVE_TABLE_LEN / (float)SAMPLERATE;
    }
    
    void setNote(uint8_t midiNote){
        this->midiNote = midiNote;
        freq = freqFromNoteNumber(midiNote, tuning);
        step = calcStep(freq);
        std::cout << " freq " << freq <<  " step "  << step << std::endl;
    }
    

    void render(SAMPLE * l, uint32_t noOfSamples)  {
        if(active){
            // std::cout << " render " << id << "noOfSamples: "  << noOfSamples << std::endl;
            for (uint32_t i = 0; i < noOfSamples; ++i) {
                //l[i] += (sine_wave_table[(int)pos] >> 9u); 
               // std::cout <<  l[i]  << std::endl;
                //samples[i] = (vol * sine_wave_table[pos >> 16u]) >> 8u;
               // int r = saw_wave_table[pos] >> 9u;
               // l[i] += r;
                pos += step;
                if (pos >= pos_max) pos -= pos_max;
            }
        }
    }
    
private:
   
};

#endif /* Voice_hpp */
