/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
 */
#include <stdbool.h>
//
#include "pico/stdlib.h"
#include "pico/sem.h"
//
#include "my_debug.h"
//
#include "spi.h"

void spi_irq_handler(spi_t *self) {
    // Clear the interrupt request.
    dma_hw->ints0 = 1u << self->rx_dma;
    myASSERT(!dma_channel_is_busy(self->rx_dma));
    sem_release(&self->sem);
}

// SPI Transfer: Read & Write (simultaneously) on SPI bus
//   If the data that will be received is not important, pass NULL as rx.
//   If the data that will be transmitted is not important,
//     pass NULL as tx and then the SPI_FILL_CHAR is sent out as each data
//     element.

bool spi_transfer(spi_t *some, const uint8_t *tx, uint8_t *rx, size_t length) {
    myASSERT(512 == length);
    myASSERT(tx || rx);
    // myASSERT(!(tx && rx));

    // tx write increment is already false
    if (tx) {
        channel_config_set_read_increment(&some->tx_dma_cfg, true);
    } else {
        static const uint8_t dummy = SPI_FILL_CHAR;
        tx = &dummy;
        channel_config_set_read_increment(&some->tx_dma_cfg, false);
    }
    // rx read increment is already false
    if (rx) {
        channel_config_set_write_increment(&some->rx_dma_cfg, true);
    } else {
        static uint8_t dummy = 0xA5;
        rx = &dummy;
        channel_config_set_write_increment(&some->rx_dma_cfg, false);
    }
    // Clear the interrupt request.
    dma_hw->ints0 = 1u << some->rx_dma;

    dma_channel_configure(some->tx_dma, &some->tx_dma_cfg,
                          &spi_get_hw(some->hw_inst)->dr,  // write address
                          tx,                              // read address
                          XFER_BLOCK_SIZE,  // element count (each element is of
                                            // size transfer_data_size)
                          false);           // start
    dma_channel_configure(some->rx_dma, &some->rx_dma_cfg,
                          rx,                              // write address
                          &spi_get_hw(some->hw_inst)->dr,  // read address
                          XFER_BLOCK_SIZE,  // element count (each element is of
                                            // size transfer_data_size)
                          false);           // start

    // start them exactly simultaneously to avoid races (in extreme cases
    // the FIFO could overflow)
    dma_start_channel_mask((1u << some->tx_dma) | (1u << some->rx_dma));

    /* Timeout 1 sec */
    uint32_t timeOut = 1000;
    /* Wait until master completes transfer or time out has occured. */
    bool rc = sem_acquire_timeout_ms(&some->sem, timeOut);  // Wait for notification from ISR
    if (!rc) {
        // If the timeout is reached the function will return false
        DBG_PRINTF("Notification wait timed out in %s\n", __FUNCTION__);
        return false;
    }
    dma_channel_wait_for_finish_blocking(some->tx_dma);
    dma_channel_wait_for_finish_blocking(some->rx_dma);

    myASSERT(!dma_channel_is_busy(some->tx_dma));
    myASSERT(!dma_channel_is_busy(some->rx_dma));

    return true;
}

bool my_spi_init(spi_t *some) {
    // bool __atomic_test_and_set (void *ptr, int memorder)
    // some built-in function performs an atomic test-and-set operation on the
    // byte at *ptr. The byte is set to some implementation defined nonzero
    // “set” value and the return value is true if and only if the previous
    // contents were “set”.
    if (__atomic_test_and_set(&(some->initialized), __ATOMIC_SEQ_CST))
        return true;

    // The SPI may be shared (using multiple SSs); protect it
    sem_init(&some->sem, 0, 1); 

    /* Configure component */
    // Enable SPI at 100 kHz and connect to GPIOs
    spi_init(some->hw_inst, 100 * 1000);
    spi_set_format(some->hw_inst, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);

    gpio_set_function(some->miso_gpio, GPIO_FUNC_SPI);
    gpio_set_function(some->mosi_gpio, GPIO_FUNC_SPI);
    gpio_set_function(some->sck_gpio, GPIO_FUNC_SPI);
    // ss_gpio is initialized in sd_spi_init()

    // SD cards' DO MUST be pulled up.
    gpio_pull_up(some->miso_gpio);

    // Grab some unused dma channels
    some->tx_dma = dma_claim_unused_channel(true);
    some->rx_dma = dma_claim_unused_channel(true);

    some->tx_dma_cfg = dma_channel_get_default_config(some->tx_dma);
    some->rx_dma_cfg = dma_channel_get_default_config(some->rx_dma);
    channel_config_set_transfer_data_size(&some->tx_dma_cfg, DMA_SIZE_8);
    channel_config_set_transfer_data_size(&some->rx_dma_cfg, DMA_SIZE_8);

    // We set the outbound DMA to transfer from a memory buffer to the SPI
    // transmit FIFO paced by the SPI TX FIFO DREQ The default is for the
    // read address to increment every element (in some case 1 byte -
    // DMA_SIZE_8) and for the write address to remain unchanged.
    channel_config_set_dreq(&some->tx_dma_cfg, spi_get_index(some->hw_inst)
                                                   ? DREQ_SPI1_TX
                                                   : DREQ_SPI0_TX);
    channel_config_set_write_increment(&some->tx_dma_cfg, false);

    // We set the inbound DMA to transfer from the SPI receive FIFO to a
    // memory buffer paced by the SPI RX FIFO DREQ We coinfigure the read
    // address to remain unchanged for each element, but the write address
    // to increment (so data is written throughout the buffer)
    channel_config_set_dreq(&some->rx_dma_cfg, spi_get_index(some->hw_inst)
                                                   ? DREQ_SPI1_RX
                                                   : DREQ_SPI0_RX);
    channel_config_set_read_increment(&some->rx_dma_cfg, false);

    /* Theory: we only need an interrupt on rx complete,
    since if rx is complete, tx must also be complete. */

    // Configure the processor to run dma_handler() when DMA IRQ 0 is
    // asserted
    irq_set_exclusive_handler(DMA_IRQ_0, some->dma_isr);

    // Tell the DMA to raise IRQ line 0 when the channel finishes a block
    dma_channel_set_irq0_enabled(some->rx_dma, true);
    irq_set_enabled(DMA_IRQ_0, true);

    LED_INIT();
    return true;
}

/* [] END OF FILE */
