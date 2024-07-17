/* File:   radio_tx_test1.c
   Author: M. P. Hayes, UCECE
   Date:   24 Feb 2018
*/

#include "radio.h"


int get_channel(void)
{
    int channel = 0;
    
    pio_config_set(CHANNEL_SEL_0, PIO_PULLUP);
    pio_config_set(CHANNEL_SEL_1, PIO_PULLUP);

    if ((pio_input_get(CHANNEL_SEL_0) && pio_input_get(CHANNEL_SEL_1))) {
        channel = 4;
    } else if (pio_input_get(CHANNEL_SEL_0)) {
        channel = 2;
    } else if (pio_input_get(CHANNEL_SEL_1)) {
        channel = 3;
    } else {
        channel = 1;
    }
    return channel;
}


nrf24_t* radio_init (void)
{
    int channel = get_channel();
    spi_cfg_t spi_cfg =
        {
            .channel = 0,
            .clock_speed_kHz = 1000,
            .cs = RADIO_CS_PIO,
            .mode = SPI_MODE_0,
            .cs_mode = SPI_CS_MODE_FRAME,
            .bits = 8
        };
    nrf24_cfg_t nrf24_cfg =
        {
            .channel = channel,
            .address = RADIO_ADDRESS,
            .payload_size = RADIO_PAYLOAD_SIZE,
            .ce_pio = RADIO_CE_PIO,
            .irq_pio = RADIO_IRQ_PIO,
            .spi = spi_cfg,
        };
    uint8_t count = 0;
    nrf24_t *nrf;


#ifdef RADIO_POWER_ENABLE_PIO
    // Enable radio regulator if present.
    pio_config_set (RADIO_POWER_ENABLE_PIO, PIO_OUTPUT_HIGH);
    delay_ms (10);
#endif

    nrf = nrf24_init (&nrf24_cfg);
    printf ("%d\n", channel);
    return nrf;
}


int write_radio(nrf24_t *nrf, float pwml, float pwmr)
{
    if (! nrf) {
        return 1;
    } else {
        char buffer[RADIO_PAYLOAD_SIZE + 1];

        snprintf (buffer, sizeof (buffer), "%.1f %.1f\r\n", pwml, pwmr);

        if (! nrf24_write (nrf, buffer, RADIO_PAYLOAD_SIZE))
            return 0;
        else
            return 1;
    }
}



// int read_radio(nrf24_t *nrf)
// {
//     char buffer[RADIO_PAYLOAD_SIZE + 1];
//     int bumper = 0;
//     uint8_t bytes;
//     bytes = nrf24_read (nrf, buffer, RADIO_PAYLOAD_SIZE);
//     if (bytes != 0)
//     {
//         buffer[bytes] = 0;
//     }
//     printf("%s\n", buffer);
//     scanf (buffer, "%d", &bumper);    

//     return bumper;
// }

void read_radio(nrf24_t *nrf, char *buffer)
{

    uint8_t bytes;
    bytes = nrf24_read (nrf, buffer, RADIO_PAYLOAD_SIZE);
    if (bytes != 0)
    {
        buffer[bytes] = 0;
        printf("%s\n", buffer); 
    }
}
