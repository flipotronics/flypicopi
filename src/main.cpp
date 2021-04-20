/**
 * author: mat@flipotronics.com
 minicom -b 115200 -o -D /dev/ttyACM0

 https://github.com/njazz/MIDIMessageParser/blob/master/src/MIDIParser.h
 */

#include <stdio.h>
#include <math.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include "string.h"
#include <stdlib.h>

#include "hardware/clocks.h"
#include "hardware/structs/clocks.h"
#include "hardware/uart.h"
#include "hardware/irq.h"
#include "hardware/flash.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "hardware/dma.h"
#include "hardware/timer.h"
#include "hardware/spi.h"

#include "pico/stdlib.h"
#include "pico/audio_i2s.h"
#include "pico/multicore.h"
#include "pico/binary_info.h"

#include "bsp/board.h"

#include "oled.h"
#include "tusb.h"
#include "Engine.h"
#include "ParamLoader.h"

using namespace std;
// ================================================ DEFINE =========================================================================


// ================================================ Member =========================================================================

extern "C" const uint8_t splash1_data[];
const uint LED_PIN = 25;
const uint LED_PIN_MIDI = 28;
const uint8_t *flash_target_contents = (const uint8_t *) (XIP_BASE + FLASH_TARGET_OFFSET);
// RX interrupt handler
static int chars_rxed = 0;

absolute_time_t taken;
uint8_t midibuffer[32];
uint8_t bcount = 0;
uint8_t b0;
uint8_t b1;
uint8_t b2;

bool sendCutoff = false;
bool isSwitchPressed;
bool isSwitchPressed2;
bool isSEnc_a;
bool isSEnc_b;
bool isMidiLight;
bool isGate1;
bool isGate2;
bool audioOk = false;
bool isMidiConnected;
bool isMidiMounted;

void print_buf(const uint8_t *buf, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        printf("%02x", buf[i]);
        if (i % 16 == 15)
            printf("\n");
        else
            printf(" ");
    }
}

void checkFlash(){
    uint8_t random_data[FLASH_PAGE_SIZE];
    for (int i = 0; i < FLASH_PAGE_SIZE; ++i)
        random_data[i] = rand() >> 16;

    printf("Generated random data:\n");
    print_buf(random_data, FLASH_PAGE_SIZE);

    // Note that a whole number of sectors must be erased at a time.
    printf("\nErasing target region...\n");
    flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);
    printf("Done. Read back target region:\n");
    print_buf(flash_target_contents, FLASH_PAGE_SIZE);

    printf("\nProgramming target region...\n");
    flash_range_program(FLASH_TARGET_OFFSET, random_data, FLASH_PAGE_SIZE);
    printf("Done. Read back target region:\n");
    print_buf(flash_target_contents, FLASH_PAGE_SIZE);

    bool mismatch = false;
    for (int i = 0; i < FLASH_PAGE_SIZE; ++i) {
        if (random_data[i] != flash_target_contents[i])
            mismatch = true;
    }
    if (mismatch)
        printf("Programming failed!\n");
    else
        printf("Programming successful!\n");
}

void tud_mount_cb(void) {
    printf("tud_mount_cb");
}

// Invoked when device is unmounted
void tud_umount_cb(void){
    printf("tud_umount_cb");
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en){
   (void) remote_wakeup_en;
   printf("tud_suspend_cb");
}

// Invoked when usb bus is resumed
void tud_resume_cb(void){
  printf("tud_resume_cb");
}

int midiTimeOut = 0;
void handleMidiByte2(u_int8_t ch){
    midiTimeOut = 0;
    midiLightCounter = 100;
    chars_rxed++;
    #if DEBUG_SHOW_MIDI
    printf("%i %i %i %i %i\n", ch, bcount, b0, b1, b2);
    #endif 
    if(bcount==0){
        if( MIDIByteInRange(ch, 144, 160) || MIDIByteInRange(ch, 128, 144) || MIDIByteInRange(ch, 176, 192) ){
        b0 = ch;
        bcount = 1;
        return;
        }
        return;
    }
    if(bcount==1){
        b1 = ch;
        bcount = 2;
        return;
    }
    if(bcount==2){
        bcount = 0;
        b2 = ch;

        // send to Queue
        MidiEvent e;
        if ( MIDIByteInRange(b0, 144, 160)) {
           // u_int8_t channel = ev.b0 - 144 + 1;
            e.type = NOTEON;
            e.channel = b0 - 144 + 1;
        }
        if (MIDIByteInRange(b0, 128, 144)) {
            e.type = NOTEOFF;
            e.channel = b0 - 144 + 1;
        }
        if (MIDIByteInRange(b0, 176, 192)) {
            e.type = CONTROL;
            e.channel = b0 - 144 + 1;
        }
        e.b0 = b0;
        e.b1 = b1;
        e.b2 = b2;
        queue_write(e);

/*
        switch(e.type){
            case NOTEON:
            printDisplay("Note On");
            break;

            case NOTEOFF:
            printDisplay("Note Off");
            break;

            case CONTROL:
            printDisplay("Control");
            break;
        }
        */
    }
    bcount = 0; // reset state maschine
}

void on_uart_rx() {
    while (uart_is_readable(UART_ID)) {
        char b = uart_getc(UART_ID);
        handleMidiByte2(b);
    }
}

void setupVoltageTable(){
    for(int i=0;i<128;i++) {
        float freq = 440.0 * pow(2.0, ( (i + 36) - 69) / 12.0);
        printf( "note: %i \n", i);
        printf( "freq: %f \n", freq);
        mpc_voltages[i] = (uint16_t) (freq * 4096.0 / (8.0 * 12543.8539514160));
        printf( "%i \n", mpc_voltages[i]);
    }
}

// =========================================================== SCAN ===========================================================================
void scan(){
    // tud_task(); // tinyusb device task
    // isMidiMounted =  tud_mounted();
    // isMidiConnected =  tud_cdc_connected();
        midiTimeOut++;
        if(midiTimeOut > 500){
            bcount = 0;
            midiTimeOut = 0;
            // printf("miditimeout");
        }

    // setup LEDs
    isMidiLight = false;
    if(midiLightCounter > 0){
        midiLightCounter--;
        isMidiLight = true;
    }

    gpio_put(LED_PIN_MIDI, isMidiLight);
    gpio_put(GATE_PIN1, isGate1);
    gpio_put(GATE_PIN2, isGate2);

    // scan the buttons
    bool isPressedNow = gpio_get(SWITCH_PIN);
    if(isSwitchPressed && !isPressedNow){
        // printf("Switch1 fires \n");
        isMidiLight = true;
    }
    isSwitchPressed = isPressedNow;

    isPressedNow = gpio_get(SWITCH_PIN2);
    if(isSwitchPressed2 && !isPressedNow){
        // printf("Switch2 fires \n");
            isMidiLight = false;
    }
    isSwitchPressed2 = isPressedNow;

    // Scan the encoder =====================================================================
    bool a = gpio_get(ENCOCDER_PIN_1);
    bool b = gpio_get(ENCOCDER_PIN_2);

    bool haveMove = false;
    if(isSEnc_a && !a){
        // printf("isSEnc_a fires \n");
        // printf("a is: %d b is: %d\n", a, b);
        haveMove = true;
    }

    if(isSEnc_b && !b){
        // printf("isSEnc_b fires \n");
        //printf("a is: %d b is: %d\n", a, b);
        haveMove = true;
    }

    if(haveMove && a==0 && b==0){
        haveMove = false;
        // printf("a is: %d b is: %d\n", a, b);
        //printf("isSEnc_a is: %d isSEnc_bis: %d\n", isSEnc_a, isSEnc_b);

        if(isSEnc_a && controls[74] < 127){
            controls[74]++;
        }
        if(isSEnc_b && controls[74] > 0){
            controls[74]--;
        }
        control(74, controls[74]);
    }
    isSEnc_a = a;
    isSEnc_b = b;
    // Scan the encoder END =====================================================================

    // Scan keyboard
    int c = getchar_timeout_us(0);
    if (c >= 0) {
        if (c == 'q') {
            gpio_put(LED_PIN, 0);
            return;
        }
        if (c == 'f') {
            // printf("FLash Test T\n");
            checkFlash();
            return;
        }
        
        if (c == 'f') {
            struct stat sb;
            int Result = stat("/test.txt", &sb);
            printf("File Test   = %i \n", Result);
            if((Result != 0 ) || (sb.st_mode & S_IFDIR )) {
                printf("File Test  does not exist \n");
            }
            return;
        }
            if (c == 'd') {
            printf("==========================================\n");
            printf("Version: %i \n", SYNTH_VERSION );
            printf("Audio Setup %d \n", audioOk);
            printf("vol = %d \n", volume);
            
            printf("UART Received = %i \n", chars_rxed);
            printf("Time taken  = %u \n", us_to_ms(taken));
            printf("Midi Mounted = %i \n", isMidiMounted);
            printf("Midi isMidiConnected = %i \n", isMidiConnected);

            printf("CC = %i \n",  controls[74]);
            
            //setupVoltageTable();
        }
        if(c == 't'){
            testSD();
        }
        if (c == 'l') {
            lfo_Freq0 = 1.0;
            for(int i=0;i<44100;i=i+100){
                printf("LFO %i    %f \n", i,  lfo_Value0);
                        lfo_calcNext(100);
            }
        }
    }

    /*
    // read USB Midi
    int bytCount = tud_midi_available ();
    if(bytCount > 0){
        int count = tud_midi_read(midibuffer, 32);
        if(count>0){
            midiLightCounter = 100;
        }
    }
    */
}

void setupMain(){
    loadPatch(0);
    setupWavetable();
    initVoices();

    lfo_init();
    lfo_Freq0 = 3.5;
    lfo_Sync0 = true;

    stdio_init_all();

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_init(LED_PIN_MIDI);
    gpio_set_dir(LED_PIN_MIDI, GPIO_OUT);

    gpio_init(SWITCH_PIN);
    gpio_set_dir(SWITCH_PIN, GPIO_IN);
    gpio_pull_up(SWITCH_PIN);

    gpio_init(SWITCH_PIN2);
    gpio_set_dir(SWITCH_PIN2, GPIO_IN);
    gpio_pull_up(SWITCH_PIN2);

    gpio_init(SD_INSERT_PIN);
    gpio_set_dir(SD_INSERT_PIN, GPIO_IN);
    gpio_pull_up(SD_INSERT_PIN);
    
    gpio_init(ENCOCDER_PIN_1);
    gpio_set_dir(ENCOCDER_PIN_1, GPIO_IN);
    gpio_pull_up(ENCOCDER_PIN_1);

    gpio_init(ENCOCDER_PIN_2);
    gpio_set_dir(ENCOCDER_PIN_2, GPIO_IN);
    gpio_pull_up(ENCOCDER_PIN_2);

    gpio_init(GATE_PIN1);
    gpio_set_dir(GATE_PIN1, GPIO_OUT);
    gpio_pull_up(GATE_PIN1);

    gpio_init(GATE_PIN2);
    gpio_set_dir(GATE_PIN2, GPIO_OUT);
    gpio_pull_up(GATE_PIN2);

   // second dac for CV
    i2c_init(PMP_DAC_DEVICE, 400 * 1000);
    gpio_set_function(0, GPIO_FUNC_I2C);
    gpio_set_function(1, GPIO_FUNC_I2C);

    gpio_pull_up(0);
    gpio_pull_up(1);
    bi_decl(bi_2pins_with_func(0, 1, GPIO_FUNC_I2C));
    sendCutoff = 127;
    sendCutoff = false;

    isGate1 = true;
    isGate2 = true;

    // sets table for the MPC DAC
    setupVoltageTable();

    // LED on
    gpio_put(LED_PIN, 1);

    isGate1 = true;
    isGate2 = true;
    //i2c_writeDac1(mpc_voltages[controls[74]]); 

//    board_init();

   // printf("Starting USB init");
   // tusb_init();
  //  printf("Done USB init");
   // tud_connect();

    // Midi UART
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    uart_set_hw_flow(UART_ID, false, false);
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);
    uart_set_fifo_enabled(UART_ID, false);
    int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;
    irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
    irq_set_enabled(UART_IRQ, true);
    uart_set_irq_enables(UART_ID, true, false);
}

// ================================================================== Main ================================================================================
int main() {

    setupMain();

    // start Auddio Render Thread
    multicore_launch_core1(renderAudio);

    // Delay Display code
    const int16_t WAITIME = 300;
    int16_t waittimer = WAITIME;

    // Main Loop ==================================================================================================================================
    while (true) {
       scan();

        // Display Code
         if(waittimer < 1 && lastShown > 0){
            std::string msg = "Control: " + std::to_string(lastShown) + " \nValue: " + std::to_string(controls[lastShown]);
            printDisplay(msg); 
            lastShown = -1;
            waittimer = WAITIME;
        }
        waittimer--;
       // sleep_ms(1);
    }

    // We never get here
    puts("\n");
    return 0;
}

// ====================================================== END  ===============================================================================