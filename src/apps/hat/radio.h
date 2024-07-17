/** @file   config.h
    @author ME
    @date   02 June 20200
    @brief 
*/
#ifndef RADIO_H
#define RADIO_H

#include "nrf24.h"
#include "pio.h"
#include "pacer.h"
#include "stdio.h"
#include "delay.h"
#include "panic.h"

// #define RADIO_CHANNEL 1
#define RADIO_ADDRESS 0x0123456799
#define RADIO_PAYLOAD_SIZE 32

int get_channel(void);

nrf24_t* radio_init (void);

int write_radio(nrf24_t *nrf, float pwml, float pwmr);

void read_radio(nrf24_t *nrf, char *buffer);

#endif /* RADIO_H  */
