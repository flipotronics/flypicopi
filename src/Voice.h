//
//  Voice.hpp
//  TestLib
//
//  author: mat@flipotronics.com
//

#ifndef Voice_hpp
#define Voice_hpp

#include <stdio.h>
#include <ostream>
#include <iostream>

#include "Def.h"
#include "Renderer.h"

class Voice : Renderer{
public:
    float volume = 1.0;
    
    float pitchbender = 1.0;
    
    bool active = false;
    bool isOn;
    
    int id;
    int channel = 0;
    int zone;
    int midiNote;
    
    float freq;
    float step;
    float delta;
    float sine_table[SAMPLE_RATE];
    float tuning = 440.0;
    float maxBend = 1.0;
    float minBend = 1.0;
    
    void pitchBend(int val){
        if(midiNote ==0) return;
        if(val > 64){
            pitchbender = 1;
        }
        if(val > 64){
            float b = (val - 64) / 64.0;
            pitchbender = 1.0 + b *  float(maxBend) / 12.0  ;
        }
        if(val < 64){
            float b = (64 - val) / 64.0;
            pitchbender = 1.0 -  b *  float(minBend) / 12.0;
        }

        freq = tuning * pow(2.0, (midiNote - 69)/12.0) * pitchbender;
        std::cout << " freq " << freq << std::endl;
        step = freq;
    }
    
    void modWheel(int val){
        std::cout << " mod " << val << std::endl;
    }
    
    void controller(int cc, int val){
        switch(cc){
            case 7:
                volume = float(val) / 127.0;
                break;
                
            default:
                break;
        }
    }
    
    void init(){
        float m_deltaTime = 1.0 / (float)SAMPLE_RATE;
        float m_time = 0.0;
        for (int i = 0; i < SAMPLE_RATE; i++){
            float value = sin(2.0 * M_PI_2  * m_time);
            sine_table[i] = value;
            m_time += m_deltaTime;
        }
    }
    
    void setNote(int midiNote){
        this->midiNote = midiNote;
        freq = tuning * pow(2.0, (midiNote - 69)/12.0) * pitchbender;
        std::cout << " freq " << freq << std::endl;
        step = freq;
        delta = 0;
    }
    
    void render(SAMPLE * l, SAMPLE * r, int noOfSamples)  {
        if(active){
            for (int i = 0; i < noOfSamples; ++i) {
                float v = sine_table[(int)delta] * volume;
                l[i] += v;
                r[i] += v;
                delta += step;
                if (delta >= SAMPLE_RATE) {
                    delta -= SAMPLE_RATE;
                }
            }
        }
    }
    
private:
   
};

#endif /* Voice_hpp */
