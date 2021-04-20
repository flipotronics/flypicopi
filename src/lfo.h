#pragma once

#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include <stdint.h>
#include <time.h>
#include <unistd.h>

#include "Def.h"

enum LFO_SHAPE{
	LFO_SIN,
	LFO_SAW,
	LFO_TRIANGLE,
	LFO_SQUARE,
	LFO_RANDOM
};

const double TABLESIZE = 512.0; 

// Use full table for Bipolar, half table for unipolar?
float sintable[512];

float lfo_Value0 = 0;
float lfo_Value1 = 0;
float lfo_Value2 = 0;
float lfo_Value3 = 0;

float lfo_Amount0 = 0;  // Volume Impact
float lfo_Amount1 = 0;  // Filter Impact
float lfo_Amount2 = 0;  // FREQ Impact
float lfo_Amount3 = 0;  // Volume Impact

float lfo_Freq0 = 1.0; // HZ * 5 / 1000
float lfo_Freq1 = 1.0;
float lfo_Freq2 = 1.0;
float lfo_Freq3 = 1.0;

double lfo_Phase0 = 0;
float lfo_Phase1 = 0;
float lfo_Phase2 = 0;
float lfo_Phase3 = 0;

int16_t lfo_Delay0 = 0; // msec
int16_t lfo_Delay1 = 0;
int16_t lfo_Delay2 = 0;
int16_t lfo_Delay3 = 0;

bool lfo_Bipolar0 = false;
bool lfo_Bipolar1 = false;
bool lfo_Bipolar2 = false;
bool lfo_Bipolar3 = false;

bool lfo_Sync0 = false;
bool lfo_Sync1 = false;
bool lfo_Sync2 = false;
bool lfo_Sync3 = false;

LFO_SHAPE lfo_Shape0 = LFO_SIN;
LFO_SHAPE lfo_Shape1 = LFO_SIN;
LFO_SHAPE lfo_Shape2 = LFO_SIN;
LFO_SHAPE lfo_Shape3 = LFO_SIN;

 const double multiplier = TABLESIZE  / SAMLERATEF;
//const float devider = SAMLERATEF ;

void lfo_calcNext(int8_t samplesForward){

	if(lfo_Bipolar0){
		float v = sintable[(int16_t)lfo_Phase0];
		if(v < 0){
			v = 0;
		}
		if(v > 1){
			v = 1;
		}
		lfo_Value0 = v;
		lfo_Phase0 += 2.0 * lfo_Freq0 * multiplier * samplesForward;
		//printf("Phase: %f \n", lfo_Phase0);
		if(lfo_Phase0 >= TABLESIZE){
			lfo_Phase0 =- TABLESIZE;
		}
	}else{

		lfo_Value0 = DACLookup_FullSine_9Bit[(uint16_t)lfo_Phase0];
		lfo_Phase0 +=  lfo_Freq0 * multiplier * samplesForward;
		if(lfo_Phase0 >= TABLESIZE){
			lfo_Phase0 =- TABLESIZE;
		}
	}
	
	if(lfo_Bipolar1){

	}else{
		
	}
	if(lfo_Bipolar2){

	}else{
		
	}
	if(lfo_Bipolar3){

	}
	else{
		
	}
}

float lfo_displayFreq(int16_t freq){
	return freq * 5.0 / 1000.0;
}


int16_t lfo_fromDisplayFreq(float freq){
	return freq * 1000.0 / 5.0;
}

void lfo_init(){
	auto cyclesPerSample = 1.0 / SAMLERATEF;
    float angleDelta = cyclesPerSample * 2.0 * 3.141592653589793;
	float phase = 0;
	for(float i=0; i< TABLESIZE; i++){
		sintable[(int16_t)i] =  sin(phase);
		phase += angleDelta;
		//printf("sin: %f \n", sintable[i]) ;
	}
	printf("multi: %f", multiplier);
}

void lfo_noteOn(){
	if(lfo_Sync0){
		lfo_Phase0 = 0;
	}
	if(lfo_Sync1){
		lfo_Phase1 = 0;
	}
	if(lfo_Sync2){
		lfo_Phase2 = 0;
	}
	if(lfo_Sync3){
		lfo_Phase3 = 0;
	}
}

void lfo_reset0(){
	lfo_Phase0 = 0;
	lfo_Value0 = 0;
}

void lfo_reset1(){
	lfo_Phase1 = 0;
	lfo_Value1 = 0;
}

void lfo_reset2(){
	lfo_Phase2 = 0;
	lfo_Value2 = 0;
}

void lfo_reset3(){
	lfo_Phase3 = 0;
	lfo_Value3 = 0;
}

void lfo_resetLfoAll(){
	lfo_reset0();
	lfo_reset1();
	lfo_reset2();
	lfo_reset3();
}



