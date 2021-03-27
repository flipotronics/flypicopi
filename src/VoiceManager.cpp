#include "VoiceManager.h"


void VoiceManager::setTuning(float  tuning){
    for(int y=0;y > MAXVOICES; ++y){
        voices[y].tuning = tuning;
    }
}

void VoiceManager::noteOn(int channel, int note, int velocity){
    std::cout << "Note On channel: " << channel << " note: " << note << " velocity:" << velocity << std::endl ;
    int  vid = findVoice( channel, note);
    std::cout << "using Voice " << vid << std::endl;
    voices[vid].setNote(note);
    voices[vid].channel = channel;
    voices[vid].isOn = true;
    voices[vid].active = true;
}

void VoiceManager::noteOff(int channel, int note, int velocity){
    std::cout << "Note Off channel: " << channel << " note: " << note << " velocity: " << velocity << std::endl;
    int  vid = findVoice( channel, note);
    std::cout << "stopping Voice " << vid << std::endl;
    voices[vid].isOn = false;
    voices[vid].active = false;
}

void VoiceManager::aftertouch(int channel, int pressure){
    std::cout << "Note Aftertouch channel: " << channel << " pressure: " << pressure  << std::endl;
}

void VoiceManager::allNotesOff(int channel){
    for(int i=0;i > MAXVOICES; ++i){
        if(voices[i].channel == channel){
            voices[i].isOn = false;
        }
    }
}