/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>

/*

#define BLOCK_COUNT 2


static uint8_t sector_data[1024];

void testSD() {
    printf("SD Card test\n");
    int i = 0;
    int sd_pin_base = 25;
    if (sd_init_1pin() < 0){
        panic("doh");
    }

    static int block_base = 0;


    static uint32_t b[BLOCK_COUNT * 128];
    for(int div = 4; div >= 1; div--){
        uint8_t *buf = (uint8_t *)b;
        printf("-----------------------\n");
        printf("SPEED %uMB/s\n", 12/div);
        sd_set_clock_divider(div);
        printf("1 bit no crc\n");
        sd_set_wide_bus(false);
        memset(buf, 0xaa, 512);
        sd_readblocks_sync(b, block_base, BLOCK_COUNT);
        for(int byte = 0; byte < 512; byte += 16) {
            printf("%08x ", i * 512 + byte);
            for(int j = 0; j < 16; j++) printf("%02x ", buf[byte + j]);
            for(int j = 0; j < 16; j++) putchar(isprint(buf[byte + j]) ? buf[byte + j] : '.');
            printf("\n");
        }

        memset(buf, 0xaa, 512);
        printf("1 bit crc\n");
        sd_read_sectors_1bit_crc_async(b, block_base, BLOCK_COUNT);
        int status = 0;
        while (!sd_scatter_read_complete(&status));
        printf("Status: %d\n", status);

        for(i = 0; i < BLOCK_COUNT; i++){

            //if (i == BLOCK_COUNT-1)
            for(int byte = 0; byte < 512; byte += 16){
                printf("%08x ", i * 512 + byte);
                for(int j = 0; j < 16; j++) printf("%02x ", buf[byte + j]);
                for(int j = 0; j < 16; j++) putchar(isprint(buf[byte + j]) ? buf[byte + j] : '.');
                printf("\n");
            }
            printf("\n");
            buf += 512;
        }
    }
}
*/
