/**
 * author: mat@flipotronics.com
 minicom -b 115200 -o -D /dev/ttyACM0
 */

#include <stdio.h>
#include <math.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "hardware/clocks.h"
#include "hardware/structs/clocks.h"
#include "hardware/uart.h"
#include "hardware/irq.h"
#include "hardware/flash.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "hardware/dma.h"

#include "pico/stdlib.h"
#include "pico/audio_i2s.h"
#include "pico/multicore.h"
#include "pico/binary_info.h"
#include "pico/sd_card.h"
#include "bsp/board.h"

#include "oled.h"
#include "tusb.h"
#include "Engine.h"

// ================================================ DEFINE =========================================================================
#define SYNTH_VERSION 3

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

// SD Card Pins
#define SPI_BUS 1  // 1
#define PICO_SD_CLK_PIN 14 // CLK
#define PICO_SD_DATI_PIN  15 // MOSI
#define PICO_SD_DAT0_PIN 12  // MISO
#define PICO_SD_CMD_PIN 13 // CS
#define SD_INSERT_PIN 18 // CD

#define FLASH_TARGET_OFFSET (256 * 1024)
#define FLASH_PAGE_SIZE (1u << 8)
#define FLASH_SECTOR_SIZE (1u << 12)
#define FLASH_BLOCK_SIZE (1u << 16)

#define MCP4725_I2CADDR_DEFAULT (0x60) ///< Default i2c address
#define MCP4725_I2CADDR_2 (0x61)
#define MCP4725_CMD_WRITEDAC (0x40)    ///< Writes data to the DAC
#define MCP4725_CMD_WRITEDACEEPROM (0x60)
#define PMP_DAC_DEVICE i2c0
#define DSIPLAY_ADDR 0x3C

#define SWITCH_PIN 22 
#define SWITCH_PIN2 21

#define GATE_PIN1 20
#define GATE_PIN2 19

#define ENCOCDER_PIN_1 17
#define ENCOCDER_PIN_2 16

// ================================================ Member =========================================================================

extern "C" const uint8_t splash1_data[];
const uint LED_PIN = 25;
const uint LED_PIN_MIDI = 28;
const uint8_t *flash_target_contents = (const uint8_t *) (XIP_BASE + FLASH_TARGET_OFFSET);
// RX interrupt handler
static int chars_rxed = 0;

absolute_time_t taken;

uint16_t mpc_voltages[128];

uint8_t midibuffer[32];
uint8_t buf[3];
uint8_t lastCutOff = 127;
uint8_t bcount = 0;
uint8_t b0;
uint8_t b1;
uint8_t b2;
uint8_t midiLightCounter = 0;

bool sendCutoff = false;
bool isSwitchPressed;
bool isSwitchPressed2;
bool isSEnc_a;
bool isSEnc_b;
bool isMidiLight;
bool isGate1;
bool isGate2;
bool audioOk = false;

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

//=========================== Methods ======================================================================================================
void i2c_writeDac1(uint16_t val) {
    printf ("Sending cutoff %i \n" ,val);
    buf[0] = MCP4725_CMD_WRITEDAC;
    buf[1] = val / 16;
    buf[2] = (val % 16) << 4 ;
    i2c_write_blocking(PMP_DAC_DEVICE, MCP4725_I2CADDR_DEFAULT, buf, 3, false);
}

void i2c_writeDac2(uint16_t val) {
    printf ("Sending adsr %i \n" ,val);
    buf[0] = MCP4725_CMD_WRITEDAC;
    buf[1] = val / 16;
    buf[2] = (val % 16) << 4 ;
    i2c_write_blocking(PMP_DAC_DEVICE, MCP4725_I2CADDR_2, buf, 3, false);
}

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

void control(uint8_t cc, uint8_t value){
    midiLightCounter = 100;
    // Volume
    if(cc==7){
        volume = value;
    }

    // Cutoff Freq
    if(cc==74){
        if(lastCutOff != value){
            lastCutOff = value;
             i2c_writeDac2(mpc_voltages[value]); 
        }
    }
}

void handleMidiByte(u_int8_t ch){
    midiLightCounter = 100;
    chars_rxed++;
    if(ch==144){
        bcount = 1;
        b0 = ch;
            return;
    }
    if(ch==128){
        bcount = 1;
        b0 = ch;
        return;
    }
    if(ch==176){
        bcount = 1;
        b0 = ch;
        return;
    }

    if(bcount==1){
        b1 = ch;
        bcount = 2;
            return;
    }

    if(bcount==2){
        b2 = ch;
        bcount = 0;
        if(b0==144){
            if(b2 == 0){
                noteOff(b1);
                return;
            }
            noteOn(b1,b2);
        }
        if(b0==128){
            noteOff(b1);
                return;
        }
        if(b0==176){
            control(b1,b2);
                return;
        }
    }
}

void on_uart_rx() {
    while (uart_is_readable(UART_ID)) {
        handleMidiByte(uart_getc(UART_ID));
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

void testScreen() {
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

void testSD(){
    if (sd_init_1pin() < 0) {
        printf("error");
    } else {
        printf("cool");
    }
}

// ================================================================== Main ================================================================================
int main() {

    setupWavetable();
    initVoices();

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

    bool isMidiMounted = false;
    // sets table for the MPC DAC
    setupVoltageTable();

    // LED on
    gpio_put(LED_PIN, 1);

    float counter = 0.0;

    isGate1 = true;
    isGate2 = true;
    lastCutOff = 127;
    i2c_writeDac2(mpc_voltages[lastCutOff]); 

    testScreen();

    //testSD();

//    board_init();

   // printf("Starting USB init");
   // tusb_init();
  //  printf("Done USB init");
   // tud_connect();
    bool isMidiConnected;

    // Main Loop ===========================================================================================
    while (true) {
        absolute_time_t tStart = get_absolute_time();

        // tud_task(); // tinyusb device task
        // isMidiMounted =  tud_mounted();
        // isMidiConnected =  tud_cdc_connected();

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
            printf("Switch1 fires \n");
            isMidiLight = true;
        }
        isSwitchPressed = isPressedNow;

        isPressedNow = gpio_get(SWITCH_PIN2);
        if(isSwitchPressed2 && !isPressedNow){
            printf("Switch2 fires \n");
             isMidiLight = false;
        }
        isSwitchPressed2 = isPressedNow;

        // Scan the encoder
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

        if(haveMove&& a==0 && b==0){
            haveMove = false;
            printf("a is: %d b is: %d\n", a, b);
            printf("isSEnc_a is: %d isSEnc_bis: %d\n", isSEnc_a, isSEnc_b);

            if(isSEnc_a){
                if(lastCutOff < 127){
                    lastCutOff++;
                }
            }
            if(isSEnc_b && lastCutOff>0){
                lastCutOff--;
            }
            i2c_writeDac2(mpc_voltages[lastCutOff]); 
            printf("Cutoff is: %d\n", lastCutOff);
        }

        isSEnc_a = a;
        isSEnc_b = b;

        // Scan keyboard
        int c = getchar_timeout_us(0);
        if (c >= 0) {
            if (c == 'q') {
                gpio_put(LED_PIN, 0);
                break;
            }
            if (c == 't') {
                printf("FLash Test T\n");
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
                printf("vol = %d \n", volume);
               
                printf("UART Received = %i \n", chars_rxed);
                printf("Time taken  = %u \n", us_to_ms(taken));
                printf("Midi Mounted = %i \n", isMidiMounted);
                printf("Midi isMidiConnected = %i \n", isMidiConnected);
                //setupVoltageTable();
            }
        }

        // Render All Audio
        renderAudio();

        // read USB Midi
         /*
        int bytCount = tud_midi_available ();
        if(bytCount > 0){
            int count = tud_midi_read(midibuffer, 32);
            if(count>0){
                midiLightCounter = 100;
                for(int i =0; i < count;++i){
                    handleMidiByte(midibuffer[i]);
                }
            }
        }
        */

       // calculate time
        absolute_time_t tEnd = get_absolute_time();
        taken = tEnd - tStart;
    }
    puts("\n");
    return 0;
}

// ====================================================== END  ===============================================================================