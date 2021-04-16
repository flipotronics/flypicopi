// author: mat@flipotronics.com

#ifndef Def_h
#define Def_h

// Types
#define SAMPLE int32_t

// Math
#define M_PI  3.14159265358979323846


// Synth
#define VERSION "1.0.0"
#define BUILDDATE "3rd. March 2021"
#define MAXVOICES 8
#define MAXZONES 1

#define SINE_WAVE_TABLE_LEN 2048
#define SAMPLES_PER_BUFFER 256
#define SAMLERATE 44100

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
#define PICO_SD_DATO_PIN 12  // MISO
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

static uint8_t buf[3];
static int sine_wave_table[SINE_WAVE_TABLE_LEN];
static int saw_wave_table[SINE_WAVE_TABLE_LEN];

static uint8_t  lastParam;
static uint8_t  lastValue;

static uint8_t controls[128];
static int lastShown = -1;
static uint8_t midiLightCounter = 0;
static uint16_t mpc_voltages[128];

struct MidiEvent{
    uint8_t b0;
    uint8_t b1;
    uint8_t b2;
    bool isActive;
};

#endif /* Def_h */