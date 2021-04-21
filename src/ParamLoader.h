#pragma once

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//
#include "hardware/adc.h"
#include "pico/stdlib.h"
#include "hardware/rtc.h"
#include "pico/util/datetime.h"

#include "sdlib/ff.h"
#include "sdlib/sd_card.h"
#include "sdlib/f_util.h"
#include "sdlib/spi.h"
#include "sdlib/f_util.h"
#include "sdlib/ff.h"
#include "sdlib/hw_config.h"
#include "sdlib/my_debug.h"
#include "sdlib/rtc.h"
#include "sdlib/sd_card.h"

// https://github.com/carlk3/no-OS-FatFS-SD-SPI-RPi-Pico

void loadPatch(uint8_t patcj){
    controls[7] = 100; // volume
    controls[74] = 100; // cutoff
}


void format(){
    FATFS fs;           /* Filesystem object */
    FIL fil;            /* File object */
    FRESULT res;        /* API result code */
    UINT bw;            /* Bytes written */
    BYTE work[FF_MAX_SS]; /* Work area (larger is better for processing time) */

    /* Format the default drive with default parameters */
     res = f_mkfs("", 0, work, sizeof work);
    printf("Test 1: %i \n", res);

    /* Give a work area to the default drive */
   res = f_mount(&fs, "", 0);
     printf("Test 2 : %i\n", res);

    /* Create a file as new */
    res = f_open(&fil, "hello.txt", FA_CREATE_NEW | FA_WRITE);
     printf("Test 3: %i \n", res);

    /* Write a message */
   res = f_write(&fil, "Hello, World!\r\n", 15, &bw);
    printf("Test 4: %i\n", res);

    /* Close the file */
   res =   f_close(&fil);
      printf("Test 5: %i\n", res);

    /* Unregister work area */
    res =  f_mount(0, "", 0);
      printf("Test 6: %i\n", res);
}


void testSD(){

  //  spi_t config;
   char cwdbuf[FF_LFN_BUF - 12] = {0};
    FRESULT fr = f_getcwd(cwdbuf, sizeof cwdbuf);
    if (FR_OK != fr) {
        printf("f_getcwd error: (%d)\n", fr);
        return;
    }


    //sd_init_driver2();

    //format();

}
