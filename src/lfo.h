#pragma once

#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include <stdint.h>
#include <time.h>
#include <unistd.h>

enum LFO_SHAPE{
	LFO_SIN,
	LFO_SAW,
	LFO_TRIANGLE,
	LFO_SQUARE,
	LFO_RANDOM
}

const int8_t TABLESIZE = 256 * 2; // Use full table for Bipolar, half table for unipolar?
int16_t sintable[TABLESIZE];

int16_t lfo_Value0 = 0;
int16_t lfo_Value1 = 0;
int16_t lfo_Value2 = 0;
int16_t lfo_Value3 = 0;

int16_t lfo_Amount0 = 0;  // Volume Impact
int16_t lfo_Amount1 = 0;  // Filter Impact
int16_t lfo_Amount2 = 0;  // FREQ Impact
int16_t lfo_Amount3 = 0;  // Volume Impact

int16_t lfo_Freq0 = 1; // HZ * 5 / 1000
int16_t lfo_Freq1 = 1;
int16_t lfo_Freq2 = 1;
int16_t lfo_Freq3 = 1;

int16_t lfo_Phase0 = 0;
int16_t lfo_Phase1 = 0;
int16_t lfo_Phase2 = 0;
int16_t lfo_Phase3 = 0;

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

void lfo_calcNext(){

}

float lfo_displayFreq(int16_t freq){
	return freq * 5.0 / 1000.0;
}


int16_t lfo_fromDisplayFreq(float freq){
	return freq * 1000.0 / 5.0;
}

void lfo_init(){
	for(int8_t i=0;i<TABLESIZE;++i){
		sintable[i] =  sin((2.0 * 3.141592653589793 * i) / TABLESIZE),
	}
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

void lfo_resetLfoAll(){
	lfo_reset0();
	lfo_reset1();
	lfo_reset2();
	lfo_reset3();
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



