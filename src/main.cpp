/**
 * author: mathias.dietrich@gmail.com
 */

#include <stdio.h>
#include <math.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "hardware/clocks.h"
#include "hardware/structs/clocks.h"

#include "pico/stdlib.h"
#include "pico/audio_i2s.h"
#include "bsp/board.h"
#include "tusb.h"
#include "hardware/uart.h"
#include "hardware/irq.h"
#include "hardware/flash.h"
#include "hardware/i2c.h"
#include "VoiceManager.h"
#include "pico/binary_info.h"

const uint LED_PIN = 25;

#define SYNTH_VERSION 2

#define I2C_DATAPIN 9
#define I2C_CLOCKPIN 10

#define MPC0_ID 0x60    // ADA 0x62 or 0x63   - Sparkfun 0x60   
#define MPC1_ID 0x61

#define UART_ID uart1
#define BAUD_RATE 31250

// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define UART_TX_PIN 4
#define UART_RX_PIN 5
#define PARITY    UART_PARITY_NONE
#define DATA_BITS 8
#define STOP_BITS 1

#define SINE_WAVE_TABLE_LEN 2048
#define SAMPLES_PER_BUFFER 256
static float sine_wave_table[SINE_WAVE_TABLE_LEN];
static float saw_wave_table[SINE_WAVE_TABLE_LEN];

absolute_time_t taken;
int lastNote;

#define SAMPLERATE 96000

#define FLASH_TARGET_OFFSET (256 * 1024)
  
 #define FLASH_PAGE_SIZE (1u << 8)
 #define FLASH_SECTOR_SIZE (1u << 12)
 #define FLASH_BLOCK_SIZE (1u << 16)
const uint8_t *flash_target_contents = (const uint8_t *) (XIP_BASE + FLASH_TARGET_OFFSET);

#define MCP4725_I2CADDR_DEFAULT (0x60) ///< Default i2c address
#define MCP4725_CMD_WRITEDAC (0x40)    ///< Writes data to the DAC
#define MCP4725_CMD_WRITEDACEEPROM (0x60)
#define PMP_DAC_DEVICE i2c0

uint8_t buf[3];
uint16_t mpc_voltages[128];

uint8_t lastCutOff = 127;

void i2c_write_byte(uint16_t val) {
    printf ("Sending cutoff %i \n" ,val);
    buf[0] = MCP4725_CMD_WRITEDAC;
    buf[1] = val / 16;
    buf[2] = (val % 16) << 4 ;
    i2c_write_blocking(PMP_DAC_DEVICE, MCP4725_I2CADDR_DEFAULT, buf, 3, false);
}

const  uint16_t DACLookup_FullSine_9Bit[512] =
{
  2048, 2073, 2098, 2123, 2148, 2174, 2199, 2224,
  2249, 2274, 2299, 2324, 2349, 2373, 2398, 2423,
  2448, 2472, 2497, 2521, 2546, 2570, 2594, 2618,
  2643, 2667, 2690, 2714, 2738, 2762, 2785, 2808,
  2832, 2855, 2878, 2901, 2924, 2946, 2969, 2991,
  3013, 3036, 3057, 3079, 3101, 3122, 3144, 3165,
  3186, 3207, 3227, 3248, 3268, 3288, 3308, 3328,
  3347, 3367, 3386, 3405, 3423, 3442, 3460, 3478,
  3496, 3514, 3531, 3548, 3565, 3582, 3599, 3615,
  3631, 3647, 3663, 3678, 3693, 3708, 3722, 3737,
  3751, 3765, 3778, 3792, 3805, 3817, 3830, 3842,
  3854, 3866, 3877, 3888, 3899, 3910, 3920, 3930,
  3940, 3950, 3959, 3968, 3976, 3985, 3993, 4000,
  4008, 4015, 4022, 4028, 4035, 4041, 4046, 4052,
  4057, 4061, 4066, 4070, 4074, 4077, 4081, 4084,
  4086, 4088, 4090, 4092, 4094, 4095, 4095, 4095,
  4095, 4095, 4095, 4095, 4094, 4092, 4090, 4088,
  4086, 4084, 4081, 4077, 4074, 4070, 4066, 4061,
  4057, 4052, 4046, 4041, 4035, 4028, 4022, 4015,
  4008, 4000, 3993, 3985, 3976, 3968, 3959, 3950,
  3940, 3930, 3920, 3910, 3899, 3888, 3877, 3866,
  3854, 3842, 3830, 3817, 3805, 3792, 3778, 3765,
  3751, 3737, 3722, 3708, 3693, 3678, 3663, 3647,
  3631, 3615, 3599, 3582, 3565, 3548, 3531, 3514,
  3496, 3478, 3460, 3442, 3423, 3405, 3386, 3367,
  3347, 3328, 3308, 3288, 3268, 3248, 3227, 3207,
  3186, 3165, 3144, 3122, 3101, 3079, 3057, 3036,
  3013, 2991, 2969, 2946, 2924, 2901, 2878, 2855,
  2832, 2808, 2785, 2762, 2738, 2714, 2690, 2667,
  2643, 2618, 2594, 2570, 2546, 2521, 2497, 2472,
  2448, 2423, 2398, 2373, 2349, 2324, 2299, 2274,
  2249, 2224, 2199, 2174, 2148, 2123, 2098, 2073,
  2048, 2023, 1998, 1973, 1948, 1922, 1897, 1872,
  1847, 1822, 1797, 1772, 1747, 1723, 1698, 1673,
  1648, 1624, 1599, 1575, 1550, 1526, 1502, 1478,
  1453, 1429, 1406, 1382, 1358, 1334, 1311, 1288,
  1264, 1241, 1218, 1195, 1172, 1150, 1127, 1105,
  1083, 1060, 1039, 1017,  995,  974,  952,  931,
   910,  889,  869,  848,  828,  808,  788,  768,
   749,  729,  710,  691,  673,  654,  636,  618,
   600,  582,  565,  548,  531,  514,  497,  481,
   465,  449,  433,  418,  403,  388,  374,  359,
   345,  331,  318,  304,  291,  279,  266,  254,
   242,  230,  219,  208,  197,  186,  176,  166,
   156,  146,  137,  128,  120,  111,  103,   96,
    88,   81,   74,   68,   61,   55,   50,   44,
    39,   35,   30,   26,   22,   19,   15,   12,
    10,    8,    6,    4,    2,    1,    1,    0,
     0,    0,    1,    1,    2,    4,    6,    8,
    10,   12,   15,   19,   22,   26,   30,   35,
    39,   44,   50,   55,   61,   68,   74,   81,
    88,   96,  103,  111,  120,  128,  137,  146,
   156,  166,  176,  186,  197,  208,  219,  230,
   242,  254,  266,  279,  291,  304,  318,  331,
   345,  359,  374,  388,  403,  418,  433,  449,
   465,  481,  497,  514,  531,  548,  565,  582,
   600,  618,  636,  654,  673,  691,  710,  729,
   749,  768,  788,  808,  828,  848,  869,  889,
   910,  931,  952,  974,  995, 1017, 1039, 1060,
  1083, 1105, 1127, 1150, 1172, 1195, 1218, 1241,
  1264, 1288, 1311, 1334, 1358, 1382, 1406, 1429,
  1453, 1478, 1502, 1526, 1550, 1575, 1599, 1624,
  1648, 1673, 1698, 1723, 1747, 1772, 1797, 1822,
  1847, 1872, 1897, 1922, 1948, 1973, 1998, 2023
};

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

bool audioOk = false;


struct audio_buffer_pool *init_audio() {
    static audio_format_t audio_format = {
            .sample_freq = SAMPLERATE, 
            .format = AUDIO_BUFFER_FORMAT_PCM_S16,
            .channel_count = 2
    };

    static struct audio_buffer_format producer_format = {
            .format = &audio_format,
            .sample_stride = 4
    };

    struct audio_buffer_pool *producer_pool = audio_new_producer_pool(&producer_format, 6,SAMPLES_PER_BUFFER); // todo correct size
    //struct audio_buffer_pool *producer_pool = audio_new_producer_pool(&producer_format, 0,0); // todo correct size
    bool __unused ok;
    
    struct audio_i2s_config config = {
            .data_pin = 9,
            .clock_pin_base = 10,
            .dma_channel = 1,
            .pio_sm = 0,
    };

    const struct audio_format *output_format;
    output_format = audio_i2s_setup(&audio_format, &config);
    if (!output_format) {
        printf("Audio setup failed");
        panic("PicoAudio: Unable to open audio device.\n");
    }

    ok = audio_i2s_connect(producer_pool);
    assert(ok);
    printf("Audio setup ok");
    audioOk = true;
    audio_i2s_set_enabled(true);
    return producer_pool;
}

uint32_t step ;
uint vol = 64;
const int multi = 1 << 16;
float tuning = 440.0;

uint32_t calcStep(float freq){
    int multi = 1 << 16;
    return ( multi * freq * (float)SINE_WAVE_TABLE_LEN / (float)SAMPLERATE);
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

int bcount = 0;
uint8_t b0;
uint8_t b1;
uint8_t b2;
// RX interrupt handler
static int chars_rxed = 0;
bool isOn;

float freqFromNoteNumber(uint8_t number, float tuning){
    float d = (number-69) / 12.0;
    return pow(2, d) * tuning;
}

void noteOn(uint8_t midiNote, uint8_t velocity){
    printf("note On %i %i  \n",midiNote, velocity);
    float freq = freqFromNoteNumber(midiNote, 440.0);
    step = calcStep(freq);
    isOn = true;
    lastNote = midiNote;
}

void noteOff(uint8_t midiNote){
    if(midiNote==lastNote){
        isOn = false;
    }
    printf("note Off %i  \n",midiNote);
}

bool sendCutoff = false;

void control(uint8_t cc, uint8_t value){
    printf("Control  %i %i \n",cc, value);

    // Volume
    if(cc==7){
        vol = value;
    }

    // Cutoff Freq
    if(cc==74){
        if(lastCutOff != value){
            lastCutOff = value;
             i2c_write_byte(mpc_voltages[value]); 
        }
    }
}

void on_uart_rx() {
    chars_rxed++;
    while (uart_is_readable(UART_ID)) {
        uint8_t ch = uart_getc(UART_ID);
        printf("UART %i \n ", ch);
        if(ch==144){
            bcount = 1;
            b0 = ch;
             continue;
        }
        if(ch==128){
            bcount = 1;
            b0 = ch;
            continue;
        }
        if(ch==176){
            bcount = 1;
            b0 = ch;
            continue;
        }

        if(bcount==1){
            b1 = ch;
            bcount = 2;
             continue;
        }

        if(bcount==2){
            b2 = ch;
            bcount = 0;
            if(b0==144){
                if(b2 == 0){
                    noteOff(b1);
                    continue;
                }
                noteOn(b1,b2);
            }
            if(b0==128){
                noteOff(b1);
                 continue;
            }
            if(b0==176){
                control(b1,b2);
                 continue;
            }
        }
        chars_rxed++;
    }
}

float noteToFreq(int note) {
    float a = 440.0; 
    return 440.0 * pow(2.0, ( note - 69) / 12.0);
}

void setupVoltageTable(){
    for(int i=0;i<128;i++){
        float freq = noteToFreq(i + 36);
        printf( "note: %i \n", i);
        printf( "freq: %f \n", freq);
        mpc_voltages[i] = (uint16_t) (freq * 4096.0 / (8.0 * 12543.8539514160));
        printf( "%i \n", mpc_voltages[i]);
    }
}

int main() {
    stdio_init_all();

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
   
    // Midi UART
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    uart_set_hw_flow(UART_ID, false, false);
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);
    uart_set_fifo_enabled(UART_ID, false);
     // Set up a RX interrupt
    // We need to set up the handler first
    // Select correct interrupt for the UART we are using
    int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;

    // And set up and enable the interrupt handlers
    irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
    irq_set_enabled(UART_IRQ, true);

    // Now enable the UART to send interrupts - RX only
    uart_set_irq_enables(UART_ID, true, false);
    step = calcStep(440);
    
   // second dac for CV
    i2c_init(PMP_DAC_DEVICE, 400 * 1000);
    gpio_set_function(0, GPIO_FUNC_I2C);
    gpio_set_function(1, GPIO_FUNC_I2C);
    gpio_pull_up(0);
    gpio_pull_up(1);
    bi_decl(bi_2pins_with_func(0, 1, GPIO_FUNC_I2C));
    sendCutoff = 127;
    sendCutoff = false;
    //i2c_write_byte( (int)lastCutOff * 32); 
    //tusb_init();

    for (int i = 0; i < SINE_WAVE_TABLE_LEN; i++) {
        sine_wave_table[i] = 32767.0 * cosf(i * 2.0 * (float) (M_PI / SINE_WAVE_TABLE_LEN));
        saw_wave_table[i] = -32767.0 + i * 32767 / SINE_WAVE_TABLE_LEN;
    }

    int multi = 1 << 16;
    int pos = 0;
    uint32_t pos_max = 0x10000 * SINE_WAVE_TABLE_LEN;

    int lastNote = 0;
    bool isMidiMounted = false;
    // sets table for the MPC DAC
    setupVoltageTable();

    // Init Audio
    struct audio_buffer_pool *ap = init_audio();
    printf("audio instance %i", ap);
    // LED on
    gpio_put(LED_PIN, 1);

    float counter = 0.0;
  
// Main Loop ===========================================================================================
    while (true) {
        absolute_time_t tStart = get_absolute_time();
        //tud_task(); // tinyusb device task

        isMidiMounted =  tud_mounted();
        
        // Scan keyboard
        int c = getchar_timeout_us(0);
        if (c >= 0) {
            if (c == '1' ) lastNote = 1;
            if (c == '2' ) lastNote = 2;
            if (c == '3' ) lastNote = 3;
            if (c == '4' ) lastNote = 4;
            if (c == '5' ) lastNote = 5;
            if (c == '6' ) lastNote = 6;
            if (c == '7' ) lastNote = 7;
            if (c == '8' ) lastNote = 8;
            if (c == '9' ) lastNote = 9;
            if (c == '-' && vol) vol -= 1;
            if ((c == '=' || c == '+') && vol < 127) vol += 1;
            if (c == 'a' && step > 0x10000) step -= 0x10000;
            if (c == 's' && step < (SINE_WAVE_TABLE_LEN / 16) * 0x20000) step += 0x10000;
            if (c == 'q') {
                gpio_put(LED_PIN, 0);
                break;
            }
            if (c == 't') {
                printf("ÜREP FLash Test T\n");
                checkFlash();
                continue;
            }
            
            if (c == 'f') {
               struct stat sb;
               int Result = stat("/test.txt", &sb);
               printf("File Test   = %i \n", Result);
               if((Result != 0 ) || (sb.st_mode & S_IFDIR )) {
                    printf("File Test  does not exist \n");
               }
               continue;
            }
             if (c == 'd') {
                printf("==========================================\n");
                printf("Version: %i \n", SYNTH_VERSION );
                printf("Audio Setup %d \n", audioOk);
                printf("vol = %d, step = %i  \n", vol, step);
                printf("Midi Mounted = %i \n", isMidiMounted);
                printf("UART Received = %i \n", chars_rxed);
                printf("Time taken  = %u \n", us_to_ms(taken));
                setupVoltageTable();
            }
        }else{
            // lastNote = -1;
        }

        uint8_t packet[4];
        struct audio_buffer *buffer = take_audio_buffer(ap, true);
        int32_t *samples = (int32_t *) buffer->buffer->bytes;
        int noOfSc = 2;
        int v = vol / noOfSc;

        // AUDIO LOOP
        for (uint i = 0; i < buffer->max_sample_count ; i++) {
            if(isOn){
               samples[i] = (v * ((int)sine_wave_table[pos >> 16u])) >> 7u; 
               int r = (v * ((int)saw_wave_table[pos >> 16u]))  >> 7u;
               samples[i] += r;
            }else{
                samples[i] = 0;
            }
            pos += step;
            if (pos >= pos_max) pos -= pos_max;
        }
        buffer->sample_count = buffer->max_sample_count;
        give_audio_buffer(ap, buffer);

        absolute_time_t tEnd = get_absolute_time();
       taken = tEnd - tStart;
    }
    puts("\n");
    return 0;
}

