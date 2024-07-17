
/** @file   config.h
    @author ME
    @date   02 June 20200
    @brief 
*/
#ifndef HAT_H
#define HAT_H

#include "pio.h"
#include "delay.h"
#include "target.h"
#include "pacer.h"
#include "usb_serial.h"
#include "adxl345.h"
#include "panic.h"
#include "piezo.h"
#include "piezo_beep.h"
#include <stdlib.h>
#include "radio.h"
#include "string.h"
#include "led_tape.h"
#include "adc.h"
// #include "buzzer.h"
/*
 * NOTE: you must define ADXL345_ADDRESS in target.h for this to compile.
 * Currently set to 0x1D
 */
#ifndef ADXL345_ADDRESS
#error ADXL345_ADDRESS must be defined
#endif

#define PACER_RATE 10
#define ACCEL_POLL_RATE 10
#define MAX_Y 2.5
#define MAX_X 2.5
#define NUM_ADC_SAMPLES 10
#define MAX_ADC(n) (2^n)-1
#define ADC_VREF 3.3

static twi_cfg_t adxl345_twi_cfg =
{
    .channel = TWI_CHANNEL_0,
    .period = TWI_PERIOD_DIVISOR (100000), // 100 kHz
    .slave_addr = 0
};

static const piezo_cfg_t piezo_config =
{
    .pio = BUZZ_PIN
};

static const adc_cfg_t adc_cfg =
{
    .bits = 12,
    .channels = BIT (ADC_CHANNEL_3),
    .trigger = ADC_TRIGGER_SW,
    .clock_speed_kHz = 1000
};


float *duty_cycle (float y, float x);

int main (void);

#endif /* HAT_H  */
