#include<stdio.h>   
#include "oled.h"

extern "C" const uint8_t ssd1306_font6x8[];
#define SET_CONTRAST 0x81
#define SET_ENTIRE_ON 0xA4
#define SET_NORM_INV 0xA6
#define SET_DISP 0xAE
#define SET_MEM_ADDR 0x20
#define SET_COL_ADDR 0x21
#define SET_PAGE_ADDR 0x22
#define SET_DISP_START_LINE 0x40
#define SET_SEG_REMAP 0xA0
#define SET_MUX_RATIO 0xA8
#define SET_COM_OUT_DIR 0xC0
#define SET_DISP_OFFSET 0xD3
#define SET_COM_PIN_CFG 0xDA	// s10.1.18 page 40
#define SET_DISP_CLK_DIV 0xD5
#define SET_PRECHARGE 0xD9
#define SET_VCOM_DESEL 0xDB
#define SET_CHARGE_PUMP 0x8D
#define I2C_PORT i2c0

static uint8_t height = 64;
//const uint8_t SID = (height == 64) ? 0x3C : 0x3D; // different height displays have different addr
const uint8_t SID = 0x3C ; // different height displays have different addr

const uint8_t width = 128;
//const int pages = height / 8;
//const bool external_vcc = false;

int pages() { return height/8; }

//uint8_t scr[pages*width+1]; // extra byte holds data send instruction
uint8_t scr[1025]; // being: 8 pages (max) * 128 width + 1 I2C command byte

void write_cmd(uint8_t cmd);

void fill_scr(uint8_t v){
	memset(scr, v, sizeof(scr));
}

void send_data(uint8_t* data, int nbytes){
	i2c_write_blocking(I2C_PORT, SID, data, nbytes, false);
}

void send2(uint8_t v1, uint8_t v2) {
	uint8_t buf[2];
	buf[0] = v1;
	buf[1] = v2;
	send_data(buf, 2);
}

void show_scr() {
	write_cmd(SET_MEM_ADDR); // 0x20
	write_cmd(0b01); // vertical addressing mode

	write_cmd(SET_COL_ADDR); // 0x21
	write_cmd(0);
	write_cmd(127);

	write_cmd(SET_PAGE_ADDR); // 0x22
	write_cmd(0);
	write_cmd(pages()-1);


	scr[0] = 0x40; // the data instruction	
	int size = pages()*width +1;
	send_data(scr, size);
}

void write_cmd(uint8_t cmd)  { 
	send2(0x80, cmd);
}

void poweroff() { write_cmd(SET_DISP | 0x00); }
void poweron() { write_cmd(SET_DISP | 0x01); }
void contrast(uint8_t contrast) { write_cmd(SET_CONTRAST); write_cmd(contrast); }
void invert(uint8_t invert) { write_cmd(SET_NORM_INV | (invert & 1)); }

static void init_i2c(){
	// This example will use I2C0 on GPIO4 (SDA) and GPIO5 (SCL)
	i2c_init(I2C_PORT, 100 * 1000);
	gpio_set_function(4, GPIO_FUNC_I2C);
	gpio_set_function(5, GPIO_FUNC_I2C);
	gpio_pull_up(4);
	gpio_pull_up(5);
}

void init_display(int h)
{
	height = h;
	static uint8_t cmds[] = {
		SET_DISP | 0x00,  // display off 0x0E | 0x00

		SET_MEM_ADDR, // 0x20
		0x00,  // horizontal

		//# resolution and layout
		SET_DISP_START_LINE | 0x00, // 0x40
		SET_SEG_REMAP | 0x01,  //# column addr 127 mapped to SEG0

		SET_MUX_RATIO, // 0xA8
		(uint8_t)(height - 1),

		SET_COM_OUT_DIR | 0x08,  //# scan from COM[N] to COM0  (0xC0 | val)
		SET_DISP_OFFSET, // 0xD3
		0x00,

		//SET_COM_PIN_CFG, // 0xDA
		//0x02 if self.width > 2 * self.height else 0x12,
		//width > 2*height ? 0x02 : 0x12,
		SET_COM_PIN_CFG, (uint8_t)(height == 32 ? 0x02 : 0x12),

		//# timing and driving scheme
		SET_DISP_CLK_DIV, // 0xD5
		0x80,

		SET_PRECHARGE, // 0xD9
		//0x22 if self.external_vcc else 0xF1,
		//external_vcc ? 0x22 : 0xF1,
		0xF1,

		SET_VCOM_DESEL, // 0xDB
		//0x30,  //# 0.83*Vcc
		0x40, // changed by mcarter

		//# display
		SET_CONTRAST, // 0x81
		0xFF,  //# maximum

		SET_ENTIRE_ON,  //# output follows RAM contents // 0xA4
		SET_NORM_INV,  //# not inverted 0xA6

		SET_CHARGE_PUMP, // 0x8D
		//0x10 if self.external_vcc else 0x14,
		//external_vcc ? 0x10 : 0x14,
		0x14,

		SET_DISP | 0x01
	};

	// write all the commands
	for(int i=0; i<sizeof(cmds); i++)
		write_cmd(cmds[i]);
	fill_scr(0);
	show_scr();
}

void draw_pixel(int16_t x, int16_t y, int color) 
{
	if(x<0 || x >= width || y<0 || y>= height) return;
	int page = y/8;
	//page = y/pages;
	int bit = 1<<(y % 8);
	int xincr = 8;
	xincr =	height/8;
	uint8_t* ptr = scr + x*xincr + page  + 1; 

	switch (color) {
		case 1: // white
			*ptr |= bit;
			break;
		case 0: // black
			*ptr &= ~bit;
			break;
		case -1: //inverse
			*ptr ^= bit;
			break;
	}
}

void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color)  {
	int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
	uint8_t byte = 0;
	for (int16_t j = 0; j < h; j++, y++) {
		for (int16_t i = 0; i < w; i++) {
			if (i & 7)
				byte <<= 1;
			else
				byte = bitmap[j * byteWidth + i / 8];
			if (byte & 0x80)
				draw_pixel(x + i, y, color);
		}
	}
}

void draw_letter_at(uint8_t x, uint8_t y, char c, int fontMultyplier)
{
	if(c< ' ' || c>  0x7F) c = '?'; // 0x7F is the DEL key

	int offset = 4 + (c - ' ' )*6;
	for(int col = 0 ; col < 6; col++) {
		uint8_t line =  ssd1306_font6x8[offset+col];
		for(int row =0; row <8; row++) {
			draw_pixel(x+col, y+row, line & 1);
			line >>= 1;
		}
	}

	for(int row = 0; row<8; row++) {
		draw_pixel(x+6, y+row, 0);
		draw_pixel(x+7, y+row, 0);
	}
}


// void draw_letter(char c) { draw_letter_at(0, 0, c); }

static int cursorx = 0, cursory = 0;

void ssd1306_print(const char* str, int fontMultyplier)
{
	char c;
	while(c = *str) {
		str++;
		if(c == '\n') {
			cursorx = 0;
			cursory += 8;
			continue;
		}
		draw_letter_at(cursorx, cursory, c, fontMultyplier);
		cursorx += 8;
	}
}

void setCursorx(int x)
{
	const int pos = 8;
	cursorx = pos * x;
}

void setCursory(int y)
{
	const int pos = 8;
	cursory = pos * y;
}

