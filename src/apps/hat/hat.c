/* File:   adxl345_PWM.c
   Author: Haruka Yamamoto (hya73), Armin Ehsan Kya (aeh96)
   Date:   1 May 2024
   Descr:  Read from an ADXL345 accelerometer and write its output to the USB serial as PWM.
*/

#include "hat.h"


float *duty_cycle (float y, float x)
{
    float *duty_cycle_percent = (float *)malloc(2*sizeof(float));
    if (duty_cycle_percent != NULL) {
    duty_cycle_percent[0] = 0.0;
    duty_cycle_percent[1] = 0.0;
    }
    
    if ((y > 20)) {
        duty_cycle_percent[0] = (y/MAX_Y) + ((x/MAX_X)/2.25);
        duty_cycle_percent[1] = (y/MAX_Y) - ((x/MAX_X)/2.25);
    } else if (y < -20 ) {
        duty_cycle_percent[0] = (y/MAX_Y) + ((x/MAX_X)/2.25);
        duty_cycle_percent[1] = (y/MAX_Y) - ((x/MAX_X)/2.25);
    } else {
        duty_cycle_percent[0] = 0;
        duty_cycle_percent[1] = 0;
    }
    if (x > 120) {
        duty_cycle_percent[0] = (x / MAX_X)/3.5;
        duty_cycle_percent[1] = 0; // Stop right wheel
    } else if (x < -120) {
        duty_cycle_percent[1] = (-x / MAX_X)/3.5;
        duty_cycle_percent[0] = 0; // Stop left wheel
    }
    if (duty_cycle_percent[0] > 100) {
        duty_cycle_percent[0] = 100;
    } else if (duty_cycle_percent[0] < -100) {
        duty_cycle_percent[0] = -100;
    }

    if (duty_cycle_percent[1] > 100) {
        duty_cycle_percent[1] = 100;
    } else if (duty_cycle_percent[1] < -100) {
        duty_cycle_percent[1] = -100;
    }


    return duty_cycle_percent;
}

float get_adc(adc_t adc)
{
    uint16_t data[1];
    float adc_voltage = 0;
    int read_adc;
    adc_read (adc, data, sizeof (data));
    read_adc = data[0];
    adc_voltage = (read_adc / (float)4095) * ADC_VREF;
    return read_adc;
}


int
main (void)
{
    twi_t adxl345_twi;
    adxl345_t *adxl345;
    char buffer[RADIO_PAYLOAD_SIZE + 1];
    int ticks = 0;
    int count = 0; 
    int test = 1;
    int bumper_state = 0;
    mcu_jtag_disable ();

    // Redirect stdio to USB serial
    usb_serial_stdio_init ();

    pio_config_set(POWER_ADC_PIO, PIO_INPUT);
    adc_t adc;
    adc = adc_init (&adc_cfg);

    pio_config_set (LED_ERROR_PIO, PIO_OUTPUT_HIGH);
    pio_config_set (LED_STATUS_PIO, PIO_OUTPUT_HIGH);

    // Initialise the TWI (I2C) bus for the ADXL345
    adxl345_twi = twi_init (&adxl345_twi_cfg);

    if (! adxl345_twi)
        panic (LED_ERROR_PIO, 1);

    // Initialise the ADXL345
    adxl345 = adxl345_init (adxl345_twi, ADXL345_ADDRESS);

    if (! adxl345)
        panic (LED_ERROR_PIO, 2);

    pacer_init (PACER_RATE);

    nrf24_t *nrf = radio_init();

    piezo_t piezo = piezo_init(&piezo_config);
    // buzzer_init();

    while (1)
    {
        pacer_wait ();
        // buzzer_bumper_hit();

        float battery_voltage = get_adc(adc);
        printf("Current voltage: %.1f\n", battery_voltage);
        if (battery_voltage < 2650) {
            /* Toggle LED.  */
            pio_output_set (LED_STATUS_PIO, PIO_OUTPUT_HIGH);
            pio_output_toggle (LED_ERROR_PIO);
            printf("VOLTAGE LOW! Current voltage: %.1f\n", battery_voltage);

        } else {
            pio_output_set (LED_ERROR_PIO, PIO_OUTPUT_HIGH);
            pio_output_toggle (LED_STATUS_PIO);

            int16_t accel[3];
            read_radio(nrf, buffer);
            sscanf(buffer, "%d", &bumper_state);
            if (bumper_state == 1) {
                piezo_beep (piezo, 500);
                bumper_state = 0;
            }

            bumper_state = 0;
            memset(buffer, 0, sizeof(buffer));

            adxl345_accel_read(adxl345, accel);

            printf ("x: %5d  y: %5d  z: %5d\n", accel[0], accel[1], accel[2]);
            float *duty_cycle_percent = duty_cycle(accel[1], accel[0]);
            printf ("Duty Cycle Left: %.2f | Duty Cycle Right: %.2f\n", duty_cycle_percent[0], duty_cycle_percent[1]);
            write_radio(nrf, duty_cycle_percent[0], duty_cycle_percent[1]);
            led_write(duty_cycle_percent[0], duty_cycle_percent[1]);
            free(duty_cycle_percent);
            read_radio(nrf, buffer);
        }

    }
}

